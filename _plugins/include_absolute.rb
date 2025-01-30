require 'net/http'
require 'uri'

module Jekyll
  class IncludeAbsoluteTag < Liquid::Tag
    def initialize(tag_name, args, tokens)
      super
      # Parse arguments (file path and optional line range)
      args = args.split(",", 2).map(&:strip)
      @file_path = args[0]
      @lines = parse_lines(args[1]) if args.size > 1 && !args[1].nil?
    end

    def render(context)
      content = if remote_file?(@file_path)
                  fetch_remote_file(@file_path)
                else
                  fetch_local_file(context)
                end

      return "Error: Unable to fetch file" unless content

      @lines ? extract_lines(content, @lines) : content
    end

    private

    # Check if file path is a URL
    def remote_file?(path)
      path.start_with?('http://', 'https://')
    end

    # Fetch file contents from a remote GitHub URL
    def fetch_remote_file(url)
      uri = URI.parse(url)
      response = Net::HTTP.get_response(uri)

      return nil unless response.is_a?(Net::HTTPSuccess)

      response.body
    rescue StandardError => e
      puts "Error fetching remote file: #{e.message}"
      nil
    end

    # Fetch file contents from local system
    def fetch_local_file(context)
      full_path = File.expand_path(@file_path, context.registers[:site].source)

      return nil unless File.file?(full_path)

      File.read(full_path)
    end

    # Parse the line range (e.g., "5-10")
    def parse_lines(lines_arg)
      lines = lines_arg.split("-").map(&:to_i)
      raise ArgumentError, "Invalid line range" if lines.size != 2 || lines.any?(&:negative?)
      lines
    end

    # Extract specific lines
    def extract_lines(content, lines)
      start_line, end_line = lines
      content_lines = content.lines

      return "Error: Line range out of bounds" if start_line > content_lines.size || start_line > end_line

      content_lines[(start_line - 1)..(end_line - 1)].join
    end
  end
end

Liquid::Template.register_tag('include_absolute', Jekyll::IncludeAbsoluteTag)
