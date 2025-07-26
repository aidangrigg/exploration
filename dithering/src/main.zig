const std = @import("std");

const img = @import("image.zig");

const Args = struct {
    input_path: [:0]const u8 = "",
    output_path: [:0]const u8 = "out.png",
    help: bool = false,
};

const CommandLineErrors = error{
    MissingInputFile,
    MissingArg,
};

fn processArgs() !Args {
    var args = std.process.args();
    _ = args.skip();

    var out = Args {};
    var input_path_set = false;

    while (args.next()) |arg| {
        if (std.mem.eql(u8, arg, "-o") or std.mem.eql(u8, arg, "--output")) {
            if (args.next()) |output_path| {
                out.output_path = output_path;
            } else {
                return CommandLineErrors.MissingArg;
            }
        } else if (std.mem.eql(u8, arg, "-h") or std.mem.eql(u8, arg, "--help")) {
            out.help = true;
        } else {
            input_path_set = true;
            out.input_path = arg;
        }
    }

    if (input_path_set) {
        return out;
    } else {
        return CommandLineErrors.MissingInputFile;
    }
}

pub fn main() !void {
    const out = std.io.getStdOut().writer();
    const args = processArgs() catch |err| {
        if (err == CommandLineErrors.MissingInputFile) {
            try out.print("Must be called with a input file.\n", .{});
        } else if (err == CommandLineErrors.MissingArg) {
            try out.print("Missing args, consult '--help' for help.\n", .{});
        }
        std.process.exit(1);
    };

    if (args.help) {
        try out.print(
                \\Usage: dither [OPTION]... [INPUT]
                \\Applies Floyd–Steinberg dithering to an input image and outputs a PNG.
                \\   -o, --output        The file to output to.
                \\   -h, --help          Show this help menu\n
                , .{}
        );
        std.process.exit(0);
    }

    const gpa = std.heap.page_allocator;
    var arena = std.heap.ArenaAllocator.init(gpa);
    defer arena.deinit();

    const allocator = arena.allocator();

    var image = try img.Image.load(args.input_path, allocator);
    defer image.deinit();

    image.dither();
    try image.write(args.output_path, allocator);
}
