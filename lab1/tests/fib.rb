require_relative "../assembler.rb"

prog do
    data do
        word 0 # 4
        word 1 # 8
        word -1 # 12
    end

    text do
        ld  x10,  4.(x9) # 16
        ld  x11,  8.(x9) # 20
        ld  x12,  12.(x9) # 24

        slti  x15,  x0,  2 # 28
        bne   x15,  x10, :base # 32

        add  x13,  x10,  x10 # 36
        add  x14,  x11,  x10 # 40

        label :loop
            add  x15,  x13,  x14 # 44
            add  x13,  x14,  x10 # 48 
            add  x14,  x15,  x10 # 52
            add  x0,   x0,   x12 # 56
            bne  x0,   x11,  :loop # 60

            add  x0,  x14,  x10 # 64
        label :base 
            ld  x8,  8.(x9) # 68
            syscall
    end
end