# typed: true

class A
  def instance_helper; end

  f = -> do
    T.cast(self, A)

    T.reveal_type(self) # error: type: `T.class_of(A)`

    self.instance_helper # error: Method `instance_helper` does not exist on `T.class_of(A)`
  end

  T.reveal_type(self) # error: type: `T.class_of(A)`

  puts f
end


class B
  extend T::Sig

  sig {params(blk: T.proc.void).void}
  def self.class_helper(&blk); end

  def instance_helper; end
end

B.class_helper {
  T.cast(self, B)

  T.reveal_type(self) # error: type: `T.class_of(<root>)`

  self.instance_helper # error: Method `instance_helper` does not exist on `T.class_of(<root>)`
}

T.reveal_type(self) # error: type: `T.class_of(<root>)`

module N
  def helper_from_N; end
end

module M
  extend T::Sig

  def helper_from_M; end

  def main
    T.cast(self, T.all(M, N))
    T.reveal_type(self) # error: type: `M`

    helper_from_M
    helper_from_N # error: Method `helper_from_N` does not exist on `M`
  end
end

module ThisSelf
  extend T::Sig

  def main
    this = T.cast(self, Kernel)

    T.reveal_type(this) # error: type: `Kernel`

    this.puts
  end
end
