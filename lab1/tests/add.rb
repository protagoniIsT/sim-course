require_relative "../assembler.rb"

prog do
    data do
        word 1
        word 0
    end

    text do 
        add x0, x1, x0
        ld x8, 4.(x9) # put 1 (= SyscallCode::PRINT) in register x8
        syscall

        ld x8, 8.(x9) # put 0 (= SyscallCode::EXIT) in register x8
        syscall
    end
end