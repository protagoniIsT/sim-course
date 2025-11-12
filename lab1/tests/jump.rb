require_relative "../assembler.rb"

prog do
    data do
        word 1
        word 0
    end 

    text do
        j :print_label
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

            ld x8, 8.(x9) # put 0 (= SyscallCode::EXIT) in register x8
            syscall
    end
end