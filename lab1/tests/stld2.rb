require_relative "../assembler.rb"

prog do
    data do
        word 10 # [4]
        word 20 # [8]
        word 30 # [12]
        word 1  # [16]
        word 0
    end

    text do
        ld  x10, 4.(x9) # 10
        ld  x11, 8.(x9) # 20 
        ld  x12, 12.(x9) # 30

        add x13, x10, x11 # 30
        add x14, x13, x12 # 60

        st  x14, 12.(x9)     # mem[12] = 60
        ld  x0, 12.(x9)     # x0 = 60

        ld  x8, 16.(x9) # PRINT, expected 60
        syscall

        ld x8, 20.(x9) # put 0 (= SyscallCode::EXIT) in register x8
        syscall
    end
end
