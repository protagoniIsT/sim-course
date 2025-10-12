require_relative "../assembler.rb"

prog do
    data do
        word 1
    end

    text do 
        add x0, x1, x0
        ld x8, 4.(x9)
        syscall
    end
end