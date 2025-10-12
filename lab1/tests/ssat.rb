require_relative "../assembler.rb"

prog do
    data do
        word 1
    end
    text do
        ssat x0, x0, 8
        ld x8, 4.(x9)
        syscall
    end
end
