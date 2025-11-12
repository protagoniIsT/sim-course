require_relative "../assembler.rb"

prog do
    data do
        word 1
        word 0
    end

    text do
        ld x10, 4.(x9)
        add x10, x0, x0       # x1 = 0
        ld  x8, 1.(x9)         # EXCEPT
        syscall

        ld x8, 8.(x9) # put 0 (= SyscallCode::EXIT) in register x8
        syscall
    end
end