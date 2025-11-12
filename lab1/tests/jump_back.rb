require_relative "../assembler.rb"

prog do
  data do
    word 0      # [4] accumulator
    word 8     # [8] count
    word 1      # [12] step
  end

  text do
    ld  x2, 8.(x9)      # 10
    ld  x3, 12.(x9)     # 1
    label :start
      add x0, x0, x3
      bne x0, x2, :start 


    ld x8, 12.(x9)         # x8 = SyscallCode::PRINT (=1)
    syscall

    ld x8, 4.(x9)         # x8 = SyscallCode::EXIT (=0)
    syscall
  end
end
