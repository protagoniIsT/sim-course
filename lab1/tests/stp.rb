require_relative "../assembler.rb"

prog do
    data do
        word 0 # 4
        word 1 # 8
    end

    text do
        slti x10, x0, 1234   # x10 = 1234
        slti x11, x0, -4   # x11 = 5678

        stp x10, x11, 12.(x9)   # mem[4] = 1234, mem[8] = 5678

        ld  x12, 12.(x9)        # x12 = mem[4] = 1234
        ld  x13, 16.(x9)        # x13 = mem[8] = 5678
        add x0, x12, x13       # x0 = 1234 + 5678 = 6912

        ld  x8, 8.(x9)        # syscall PRINT
        syscall
        
        ld  x8, 4.(x9)        # syscall EXIT
        syscall

    end
end
