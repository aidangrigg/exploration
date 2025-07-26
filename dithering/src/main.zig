const std = @import("std");

const img = @import("image.zig");
const stbiw = @cImport(@cInclude("stb_image_write.h"));

fn greyscale(px: img.Pixel) u8 {
    const r: f32 = @floatFromInt(px.r);
    const g: f32 = @floatFromInt(px.g);
    const b: f32 = @floatFromInt(px.b);
    const grey = std.math.clamp(0.299 * r + 0.587 * g + 0.114 * b, 0, 255);
    return @intFromFloat(grey);
}

fn clampToU8(val: f32) u8 {
    return @intFromFloat(@min(@max(val, 0.0), 255.0));
}

pub fn main() !void {
    var args = std.process.args();
    _ = args.skip();

    const filename = args.next();

    if (filename == null) {
        std.debug.print("ERROR: Filename must be passed in", .{});
        return error.MissingFilename;
    }

    const gpa = std.heap.page_allocator;
    var arena = std.heap.ArenaAllocator.init(gpa);
    defer arena.deinit();

    const allocator = arena.allocator();

    var image = try img.Image.load(filename.?, allocator);
    defer image.free();

    std.debug.print("Width: {}, Height: {}, Channels: {}", .{ image.width, image.height, image.channels });

    var data = std.ArrayList(u8).init(allocator);

    for (image.data.items) |pixel| {
        try data.append(greyscale(pixel));
    }

    for (0..image.height) |y| {
        for (0..image.width) |x| {
            const idx = y * image.width + x;
            const old_pixel: f32 = @floatFromInt(data.items[idx]);
            const new_pixel: f32 = if (old_pixel > 127) 255.0 else 0.0;
            data.items[idx] = clampToU8(new_pixel);
            const quant_error = old_pixel - new_pixel;

            if (x + 1 < image.width) {
                const i = idx + 1;
                const val: f32 = @floatFromInt(data.items[i]);
                data.items[i] = clampToU8(val + quant_error * 7.0 / 16.0);
            }

            if (x >= 1 and y + 1 < image.height) {
                const i = idx - 1 + image.width;
                const val: f32 = @floatFromInt(data.items[i]);
                data.items[i] = clampToU8(val + quant_error * 3.0 / 16.0);
            }

            if (y + 1 < image.height) {
                const i = idx + image.width;
                const val: f32 = @floatFromInt(data.items[i]);
                data.items[i] = clampToU8(val + quant_error * 5.0 / 16.0);
            }

            if (x + 1 < image.width and y + 1 < image.height) {
                const i = idx + 1 + image.width;
                const val: f32 = @floatFromInt(data.items[i]);
                data.items[i] = clampToU8(val + quant_error * 1.0 / 16.0);
            }
        }
    }

    _ = stbiw.stbi_write_png("./out.png", @intCast(image.width), @intCast(image.height), 1, data.items.ptr, @intCast(image.width));
}
