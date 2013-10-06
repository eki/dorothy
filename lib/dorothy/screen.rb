
class Z::Screen
  attr_reader   :style, :window, :upper, :lower
  attr_accessor :width, :height

  def initialize( zmachine )
    @zmachine = zmachine

    @width, @height = 80, 40

    self.style = :normal

    self.foreground = :black
    self.background = :white

    @default_foreground = @foreground
    @default_background = @background

    @lower = LowerWindow.new( self )
    @upper = UpperWindow.new( self )

    @window = @lower
  end

  def <<( s )
    window << s
    self
  end

  def split( height )
    upper.height = height
    upper.reset_cursor!
    upper.clear             # Only clear if v3 !!!
    @window = upper
  end

  def window=( w )
    @window = (w == :upper || w == 1) ? upper : lower
  end

  def style=( s )
    @style = s.kind_of?( Symbol ) ? STYLES[s] : s
  end

  def foreground=( c )
    return 0  if c == 0

    @foreground = COLORS.index( c ) || c
    if foreground == :default
      @foreground = @default_foreground
    end
  end

  def background=( c )
    return 0  if c == 0

    @background = COLORS.index( c ) || c
    if background == :default
      @background = @default_background
    end
  end

  def foreground
    COLORS[@foreground]
  end

  def background
    COLORS[@background]
  end

  def default_foreground
    COLORS[@default_foreground]
  end

  def default_background
    COLORS[@default_background]
  end

  def clear
    upper.clear
    lower.clear
    self
  end

  class UpperWindow
    attr_reader :screen, :height

    def initialize( screen, height=0 )
      @screen = screen

      self.height = height
    end

    def height=( h )
      if @height != h
        @height = h

        n = screen.width * height

        @chars =  [] * n
        @colors = [] * n
        @styles = [] * n

        @cursor = 0
      end
    end

    def cursor
      [@cursor / screen.width, @cursor % screen.width]
    end

    def advance_cursor!
      @cursor += 1  if @cursor + 1  < screen.width * height
    end

    def reset_cursor!
      @cursor = 0
    end

    def <<( s )
      return self  unless height > 0

      style = screen.style

      foreground = COLORS.index( screen.foreground )
      background = COLORS.index( screen.background )

      s.each_char do |c|
        @chars[@cursor]  = c
        @colors[@cursor] = [foreground, background]
        @styles[@cursor] = style

        advance_cursor!
      end

      self
    end

    def clear
      @chars.clear
      @colors.clear
      @styles.clear
      self
    end

    def to_s
      @chars.each_slice( screen.width ).map { |cs| cs.join.strip << "\n" }.join
    end

    def to_html
      i = 0
      last_colors, last_style = nil, nil
      s = ''
      texts = []

      while i < @chars.length
        if i == 0
          s << @chars[i]
          last_colors, last_style = @colors[i], @styles[i]

        elsif last_colors == @colors[i] && @styles[i] == last_style
          s << @chars[i]

        else
          texts << Text.new( s, last_style, *last_colors )

          s = @chars[i]
          last_colors, last_style = @colors[i], @styles[i]
        end

        i += 1
      end

      texts << Text.new( s, last_style, *last_colors )  unless s.empty?
      texts.map { |t| t.to_html }.join
    end
  end

  class LowerWindow
    attr_reader :screen, :output

    def initialize( screen )
      @screen = screen
      @output = []
    end

    def style;      screen.style;      end
    def foreground; screen.foreground; end
    def background; screen.background; end

    def <<( s )
      output << Text.new( s, style, foreground, background )
      self
    end

    def clear
      output.clear
      self
    end

    def remove_prompt
      if output.last.string =~ /\A\s*>\s*\Z/
        output.pop
      end
    end

    def to_s
      output.map { |t| t.to_s }.join
    end

    def to_html( opts={} )
      output.map { |t| t.to_html }.join
    end
  end

  COLORS = [:current, :default, :black, :red, :green, :yellow, :blue,
            :magenta, :cyan, :white]

  STYLES = {
    :normal  => 0,
    :reverse => 1,
    :bold    => 2,
    :italic  => 4,
    :fixed   => 8
  }

  class Text
    attr_reader :string, :style

    def initialize( string, style, foreground, background )
      @string = string

      @style = style.kind_of?( Symbol ) ? STYLES[style] : style

      @foreground = COLORS.index( foreground ) || foreground
      @background = COLORS.index( background ) || background
    end

    def styles
      return [:normal]  if @style == 0

      STYLES.select { |k,v| v != 0 && @style & v == v }.map { |k,v| k }
    end

    def foreground
      COLORS[@foreground]
    end

    def background
      COLORS[@background]
    end

    def to_s
      @string.to_s
    end

    def classes
      "#{styles.join( ' ' )} fg_#{foreground} bg_#{background}"
    end

    def to_html
      s = to_s.gsub( /\n/, '<br />' )

      %Q(<span class="#{classes}">#{s}</span>)
    end

    def inspect
      @string.inspect
    end
  end

end

