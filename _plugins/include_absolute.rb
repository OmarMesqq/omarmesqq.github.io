module Jekyll
    class IncludeAbsoluteTag < Liquid::Tag
      def initialize(tag_name, args, tokens)
        super
        # Split the arguments into file path and optional line range
        args = args.split(",", 2).map(&:strip)
        @file_path = args[0]
        @lines = parse_lines(args[1]) if args.size > 1 && !args[1].nil?
      end
  
      def render(context)
        # Resolve the absolute path of the file
        full_path = File.expand_path(@file_path, context.registers[:site].source)
  
        # Check if the file exists
        unless File.file?(full_path)
          puts "File not found: #{@file_path}"
          return "Error: File not found: #{@file_path}"
        end
  
        # Read the content of the file
        content = File.read(full_path)
  
        # Extract specific lines if a range is provided
        @lines ? extract_lines(content, @lines) : content
      end
  
      private
  
      # Parse the line range (e.g., "5-10")
      def parse_lines(lines_arg)
        lines = lines_arg.split("-").map(&:to_i)
        raise ArgumentError, "Invalid line range" if lines.size != 2 || lines.any?(&:negative?)
        lines
      end
  
      # Extract the specified lines from the content
      def extract_lines(content, lines)
        start_line, end_line = lines
        content_lines = content.lines
  
        # Ensure the line range is valid
        if start_line > content_lines.size || start_line > end_line
          return "Error: Line range out of bounds"
        end
  
        # Return only the specified lines
        content_lines[(start_line - 1)..(end_line - 1)].join
      end
    end
  end
  
  Liquid::Template.register_tag('include_absolute', Jekyll::IncludeAbsoluteTag)
  