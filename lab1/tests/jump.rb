require_relative "../assembler.rb"

prog do
    data do
        word 1
    end 

    text do
        j :label
        add  x0, x0, x0
        add  x0, x0, x0

        label :label
            add  x0, x0, x0
            add  x0, x0, x0
            add  x0, x0, x0
            add  x0, x0, x0

        label :print_label
            add  x0, x0, x0
            ld   x8, 4.(x9)
            syscall
    end
end