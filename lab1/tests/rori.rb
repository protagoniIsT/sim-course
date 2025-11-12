require_relative "../assembler.rb"

prog do
    data do
        word 1
        word 0
    end

    text do
        rori x0, x0, 3
        ld x8, 4.(x9)
        syscall

        ld x8, 8.(x9) # put 0 (= SyscallCode::EXIT) in register x8
        syscall
    end
end