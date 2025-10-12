require_relative "../assembler.rb"

prog do
    data do
        word 1
    end

    text do
        rori x0, x0, 3
        ld x8, 4.(x9)
        syscall
    end
end