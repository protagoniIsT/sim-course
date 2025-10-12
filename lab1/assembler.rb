module ISA
    OPCODES = {
        # R-TYPE:
        RTYPE: 0b000000,

        # I-TYPE:
        SLTI: 0b010110,
        ST: 0b000011,
        STP: 0b001000,
        RORI: 0b111011,
        BNE: 0b100100,
        BEQ: 0b000010,
        LD: 0b011010,
        SSAT: 0b001111,

        # J-TYPE:
        JTYPE: 0b100111
    }

    FUNCT = {
        ADD: 0b101011,
        AND: 0b101101,
        BDEP: 0b111010,
        CLS: 0b111001,
        SYSCALL: 0b111110
    }

    WORD_SIZE = 4
end

class MemRef
    attr_reader :offset, :base
    def initialize(offset, base)
        @offset = offset
        @base = base
    end
end

class Numeric
    def call(base)
        MemRef.new(self, base)
    end
end


class Assembler
    include ISA

    def initialize
        @instructions = []
        @data = []
        @labels = {}
        @pc = 0
        @state = :data
        @is_preprocessing = false
    end

    (0..31).each { |i| define_method("x#{i}") { i } }

    def preprocess(&block)
        @is_preprocessing = true
        @pc = WORD_SIZE

        instrs = %i[
            add and_ bdep cls syscall
            slti st stp rori ssat
            bne beq ld j word
        ]

        instrs.each do |m|
            self.class.class_eval do
            alias_method :"orig_#{m}", m
            define_method(m) { |*args| @pc += WORD_SIZE }
            end
        end

        instance_eval(&block)

        instrs.each do |m|
            self.class.class_eval do
            alias_method m, :"orig_#{m}"
            end
        end

        @is_preprocessing = false
    end



    def label(name)
        if @is_preprocessing
            @labels[name] = @pc
        end
    end

    def data(&block)
        @state = :data
        yield if block_given?
    end

    def text(&block)
        @state = :text
        @pc = WORD_SIZE + @data.size * WORD_SIZE unless @is_preprocessing
        yield if block_given?
    end

    def word(value)
        @data << value.to_i
    end

    def write_word_and_advance(word)
        @instructions << word.to_i
        @pc += WORD_SIZE
    end

    def parse_mem(memref) # str should be MemRef type
        [memref.offset, memref.base]
    end

    # R-TYPE:
    def add(rd, rs, rt) 
        encoded_instr = ((OPCODES[:RTYPE] & 0x3F) << 26) \
                       | ((rs & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | ((rd & 0x1F) << 11) \
                       | (FUNCT[:ADD] & 0x3F)
        write_word_and_advance(encoded_instr)
    end

    def and_(rd, rs, rt)
        encoded_instr = ((OPCODES[:RTYPE] & 0x3F) << 26) \
                       | ((rs & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | ((rd & 0x1F) << 11) \
                       | (FUNCT[:AND] & 0x3F)
        write_word_and_advance(encoded_instr)
    end

    def bdep(rd, rs1, rs2)
        encoded_instr = ((OPCODES[:RTYPE] & 0x3F) << 26) \
                       | ((rd & 0x1F) << 21) \
                       | ((rs1 & 0x1F) << 16) \
                       | ((rs2 & 0x1F) << 11) \
                       | (FUNCT[:BDEP] & 0x3F)
        write_word_and_advance(encoded_instr)
    end

    def cls(rd, rs)
        encoded_instr = ((OPCODES[:RTYPE] & 0x3F) << 26) \
                       | ((rd & 0x1F) << 21) \
                       | ((rs & 0x1F) << 16) \
                       | (FUNCT[:CLS] & 0x3F)
        write_word_and_advance(encoded_instr)
    end
        
    def syscall()
        encoded_instr = ((OPCODES[:RTYPE] & 0x3F) << 26) \
                       | (FUNCT[:SYSCALL] & 0x3F)
        write_word_and_advance(encoded_instr)
    end


    # I-TYPE
    def slti(rt, rs, imm)
        encoded_instr = ((OPCODES[:SLTI] & 0x3F) << 26) \
                       | ((rs & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | (imm & 0xFFFF)
        write_word_and_advance(encoded_instr)
    end

    def st(rt, memref)
        offset, base = parse_mem(memref)
        encoded_instr = ((OPCODES[:ST] & 0x3F) << 26) \
                       | ((base & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | (offset & 0xFFFF)
        write_word_and_advance(encoded_instr)
    end

    def stp(rt1, rt2, memref)
        offset, base = parse_mem(memref)
        encoded_instr = ((OPCODES[:STP] & 0x3F) << 26) \
                       | ((base & 0x1F) << 21) \
                       | ((rt1 & 0x1F) << 16) \
                       | ((rt2 & 0x1F) << 11) \
                       | (offset & 0x7FF)
        write_word_and_advance(encoded_instr)
    end

    def rori(rd, rs, imm)
        encoded_instr = ((OPCODES[:RORI] & 0x3F) << 26) \
                       | ((rd & 0x1F) << 21) \
                       | ((rs & 0x1F) << 16) \
                       | ((imm & 0x1F) << 11) 
        write_word_and_advance(encoded_instr)
    end

    def ssat(rd, rs, imm)
        encoded_instr = ((OPCODES[:SSAT] & 0x3F) << 26) \
                       | ((rd & 0x1F) << 21) \
                       | ((rs & 0x1F) << 16) \
                       | ((imm & 0x1F) << 11) 
        write_word_and_advance(encoded_instr)
    end

    def bne(rs, rt, label)
        target = @labels[label] or raise "Unknown label: #{label}"
        offset = ((target - @pc) / WORD_SIZE)
        encoded_instr = ((OPCODES[:BNE] & 0x3F) << 26) \
                       | ((rs & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | (offset & 0xFFFF)
        write_word_and_advance(encoded_instr)
    end

    def beq(rs, rt, label)
        target = @labels[label] or raise "Unknown label: #{label}"
        offset = ((target - @pc) / WORD_SIZE)
        encoded_instr = ((OPCODES[:BEQ] & 0x3F) << 26) \
                       | ((rs & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | (offset & 0xFFFF)
        write_word_and_advance(encoded_instr)
    end

    def ld(rt, memref)
        offset, base = parse_mem(memref)
        encoded_instr = ((OPCODES[:LD] & 0x3F) << 26) \
                       | ((base & 0x1F) << 21) \
                       | ((rt & 0x1F) << 16) \
                       | (offset & 0xFFFF)
        write_word_and_advance(encoded_instr)
    end

    # J-TYPE
    def j(label)
        target = @labels[label] or raise "Unknown label: #{label}"
        index = target / WORD_SIZE
        encoded_instr = ((OPCODES[:JTYPE] & 0x3F) << 26) \
                       | (index & 0x3FFFFFF)
        write_word_and_advance(encoded_instr)
    end


    def save_binary(filename = "#{File.basename($0, '.rb')}.bin")
        File.open(filename, "wb") do |f|
            entry = (@data.size + 1) * WORD_SIZE
            f.write([entry].pack("L<"))
            @data.each { |w| f.write([w].pack("L<")) }
            @instructions.each { |w| f.write([w].pack("L<")) }
        end
        puts "[Assembler] Saved binary to #{filename}"
    end
end

def prog(&block)
    asm = Assembler.new
    asm.preprocess(&block)
    asm.instance_eval(&block)
    asm.save_binary
end
