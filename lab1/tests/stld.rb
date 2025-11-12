require_relative "../assembler.rb"

prog do
    data do
        word 1234 # 4
        word 1 # 8
        word 0 # 12
    end

    text do
        ld  x10, 4.(x9)      # x10 = mem[4] = 1234
        st  x10, 16.(x9)     # mem[12] = x10 (1234)
        ld  x11, 16.(x9)      # x11 = mem[4]
        add x0, x0, x11      # x0 = 1234
        ld  x8, 8.(x9)       # PRINT, expected 1234      
        syscall

        ld x8, 12.(x9) # put 0 (= SyscallCode::EXIT) in register x8
        syscall
    end
end
