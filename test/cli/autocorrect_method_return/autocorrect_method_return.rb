# typed: true

extend T::Sig

  sig {returns(String)}
  def implicit_return_non_empty_cont_block
    puts nil
  # ^^^^^^^^ error: Expected `String` but found `NilClass` for method result type
  end
sig {returns(String)}
def double_return
  return puts nil
end

sig {params(x: Module).returns(String)}
def initialized_twice(x)
  if T.unsafe(nil)
    res = x.name
  else
    res = x.name
  end
  return res
end

sig {returns(String)}
def implicit_return_via_else
  if T.unsafe(nil)
    return 'yep'
  end
end
