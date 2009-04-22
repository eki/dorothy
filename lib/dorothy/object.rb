
class Z::Object
  attr_reader :memory, :number

  def initialize( memory, number )
    @memory, @number = memory, number
  end

  def parent?
    !! parent
  end

  def children?
    !! child
  end

  def siblings?
    ! siblings.empty?
  end

  def children
    return []  unless children?

    [child] + child.siblings
  end

  def siblings
    return []  unless parent?

    first_child = parent.child

    a, s = [first_child], first_child
    a << s while s = s.sibling

    a.reject! { |o| o.number == number }

    a
  end

  def root
    return self  unless parent?

    p = parent
    p = p.parent while p.parent
    p
  end

  def inspect
    "#<Z::Object number: #{number} name: #{name}>"
  end
end

