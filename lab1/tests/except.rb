require_relative "../assembler.rb"

prog do
    data do
        word 1
    end

    text do
        ld x10, 4.(x9)
        add x10, x0, x0       # x1 = 0
        ld  x8, 1.(x9)         # EXCEPT
        syscall
    end
end