const std = @import("std");

const stbi = @cImport(@cInclude("stb_image.h"));
const stbiw = @cImport(@cInclude("stb_image_write.h"));

pub const Pixel = struct {
    r: u8,
    g: u8,
    b: u8,
    a: u8,

    pub fn fromSlice(slice: []u8) !Pixel {
        switch (slice.len) {
            4 => {
                return .{
                    .r = slice[0],
                    .g = slice[1],
                    .b = slice[2],
                    .a = slice[3],
                };
            },
            3 => {
                return .{
                    .r = slice[0],
                    .g = slice[1],
                    .b = slice[2],
                    .a = 1,
                };
            },
            else => {
                return error.Bounds;
            },
        }
    }
};

pub const Image = struct {
    width: usize,
    height: usize,
    channels: u8,
    data: std.ArrayList(Pixel),
    c_data: []u8,

    pub fn free(self: Image) void {
        stbi.stbi_image_free(self.c_data.ptr);
        self.data.deinit();
    }

    pub fn load(path: [:0]const u8, alloc: std.mem.Allocator) !Image {
        var width: c_int = undefined;
        var height: c_int = undefined;
        var channels: c_int = undefined;

        var c_data = stbi.stbi_load(path, &width, &height, &channels, 0);

        var img: Image = undefined;
        img.width = @intCast(width);
        img.height = @intCast(height);
        img.channels = @intCast(channels);
        img.data = std.ArrayList(Pixel).init(alloc);

        img.c_data = c_data[0 .. img.height * img.width * img.channels];

        var i: usize = 0;
        while (i < img.c_data.len) : (i += img.channels) {
            const slice = img.c_data[i .. i + img.channels];
            const pixel = try Pixel.fromSlice(slice);
            try img.data.append(pixel);
        }

        return img;
    }

    pub fn write(self: Image, alloc: std.mem.Allocator) !void {
        var data = std.ArrayList(u8).init(alloc);
        for (self.data.items) |pixel| {
            try data.append(if (pixel.r == 1.0) 255 else 0);
        }

        std.debug.print("data size: {}\n", .{data.items.len});
        std.debug.print("expected size: {}\n", .{self.height * self.width});

        _ = stbiw.stbi_write_bmp("./out.bmp", @intCast(self.width), @intCast(self.height), 1, data.items.ptr);
    }
};
