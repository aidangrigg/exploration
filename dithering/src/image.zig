const std = @import("std");

const stbi = @cImport(@cInclude("stb_image.h"));
const stbiw = @cImport(@cInclude("stb_image_write.h"));

pub const Pixel = struct {
    rgb: [3]f32,

    pub fn init(data: []u8) Pixel {
        std.debug.assert(data.len != 0 and data.len < 4);

        var px: Pixel = undefined;

        for (0..3) |i| {
            const idx = if (data.len <= 2) 0 else i;
            px.rgb[i] = @floatFromInt(data[idx]);
        }

        return px;
    }

    pub fn r(self: Pixel) f32 {
        return self.rgb[0];
    }

    pub fn g(self: Pixel) f32 {
        return self.rgb[1];
    }

    pub fn b(self: Pixel) f32 {
        return self.rgb[2];
    }

    pub fn luminosity(self: Pixel) f32 {
        return std.math.clamp(0.299 * self.r() + 0.587 * self.g() + 0.114 * self.b(), 0, 255);
    }
};

pub const Image = struct {
    width: usize,
    height: usize,
    channels: u8,
    data: std.ArrayList(Pixel),

    pub fn deinit(self: Image) void {
        self.data.deinit();
    }

    pub fn load(path: [:0]const u8, alloc: std.mem.Allocator) !Image {
        var width: c_int = undefined;
        var height: c_int = undefined;
        var channels: c_int = undefined;

        var c_data = stbi.stbi_load(path, &width, &height, &channels, 0);
        defer stbi.stbi_image_free(c_data);

        var img: Image = undefined;
        img.width = @intCast(width);
        img.height = @intCast(height);
        img.channels = @intCast(channels);
        img.data = try std.ArrayList(Pixel).initCapacity(alloc, img.width * img.height);

        var i: usize = 0;
        while (i < img.width * img.height) : (i += 1) {
            const j = i * img.channels;
            const slice = c_data[j .. j + img.channels];
            img.data.appendAssumeCapacity(Pixel.init(slice));
        }

        return img;
    }

    pub fn find_closest_palette_color(px: Pixel) Pixel {
        if (px.luminosity() > 127.0) {
            return .{
                .rgb = .{ 255, 255, 255 },
            };
        } else {
            return .{
                .rgb = .{ 0, 0, 0 },
            };
        }
    }

    fn apply_quant(px: *Pixel, quant_r: f32, quant_g: f32, quant_b: f32, factor: f32, div: f32) void {
        px.rgb[0] += quant_r * factor / div;
        px.rgb[1] += quant_g * factor / div;
        px.rgb[2] += quant_b * factor / div;
    }

    pub fn dither(self: *Image) void {
        for (0..self.height) |y| {
            for (0..self.width) |x| {
                const idx = y * self.width + x;
                const old_pixel = self.data.items[idx];
                const new_pixel = find_closest_palette_color(old_pixel);
                self.data.items[idx] = new_pixel;
                const quant_r = old_pixel.r() - new_pixel.r();
                const quant_g = old_pixel.g() - new_pixel.g();
                const quant_b = old_pixel.b() - new_pixel.b();

                if (x + 1 < self.width) {
                    const val = &self.data.items[idx + 1];
                    apply_quant(val, quant_r, quant_g, quant_b, 7, 16);
                }

                if (x >= 1 and y + 1 < self.height) {
                    const val = &self.data.items[idx - 1 + self.width];
                    apply_quant(val, quant_r, quant_g, quant_b, 3, 16);
                }

                if (y + 1 < self.height) {
                    const val = &self.data.items[idx + self.width];
                    apply_quant(val, quant_r, quant_g, quant_b, 5, 16);
                }

                if (x + 1 < self.width and y + 1 < self.height) {
                    const val = &self.data.items[idx + 1 + self.width];
                    apply_quant(val, quant_r, quant_g, quant_b, 1, 16);
                }
            }
        }
    }

    fn clamptoU8(val: f32) u8 {
        return @intFromFloat(std.math.clamp(val, 0, 255));
    }

    pub fn write(self: Image, path: [:0]const u8, alloc: std.mem.Allocator) !void {
        var data = std.ArrayList(u8).init(alloc);
        for (self.data.items) |px| {
            try data.append(clamptoU8(px.r()));
            try data.append(clamptoU8(px.g()));
            try data.append(clamptoU8(px.b()));
        }

        _ = stbiw.stbi_write_png(path, @intCast(self.width), @intCast(self.height), 3, data.items.ptr, @intCast(self.width * 3));
    }
};
