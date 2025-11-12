require_relative "../assembler.rb"

prog do
    data do
        word 1
        word 0
    end

    text do 
        add x0, x1, x0
    end
end