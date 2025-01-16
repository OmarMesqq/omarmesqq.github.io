module Jekyll
    class IncludeAbsoluteTag < Liquid::Tag
      def initialize(tag_name, file_path, tokens)
        super
        @file_path = file_path.strip
      end
  
      def render(context)
        full_path = File.expand_path(@file_path, context.registers[:site].source)
        if File.file?(full_path)
          File.read(full_path)
        else
          puts "File not found: #{@file_path}"
          nil
        end
      end
    end
  end
  
  Liquid::Template.register_tag('include_absolute', Jekyll::IncludeAbsoluteTag)
  