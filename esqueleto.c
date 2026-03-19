/**
 * #################################
 * # Esqueleto do processador em C #
 * #################################
*/
#include <stdio.h>
#include <stdint.h>

uint8_t mem[256] = {0};
uint8_t reg[32] = {0};
uint8_t pc = 0, zf = 0, running = 1;
int ciclo = 0;

void fetch(uint8_t *op, uint8_t *a, uint8_t *b) {
    *op = mem[pc]; *a = mem[pc+1]; *b = mem[pc+2];
    pc += 3;
}

void decode_execute(uint8_t op, uint8_t a, uint8_t b) {
    switch (op) {
        case 0x01: reg[a] = mem[b]; break;
        case 0x02: mem[b] = reg[a]; break;
        case 0x03: reg[a] = reg[a] + reg[b]; break;
        case 0x04: reg[a] = reg[a] - reg[b]; break;
        case 0x05: reg[a] = b; break;
        case 0x06: zf = (reg[a] == reg[b]) ? 1 : 0; break;
        case 0x07: pc = a; break;
        case 0x08: if (zf) pc = a; break;
        case 0x09: if (!zf) pc = a; break;
        case 0x0A: running = 0; break;
    }
}

void trace(uint8_t op, uint8_t a, uint8_t b) {
    const char *nomes[] = {"","LOAD","STORE","ADD",
        "SUB","MOV","CMP","JMP","JZ","JNZ","HALT"};
    printf("Ciclo %d: %-5s %d,%d | R16=%3d R17=%3d"
           " R18=%3d R19=%3d R20=%3d R21=%3d R22=%3d R23=%3d| PC=%3d ZF=%d\n",
           ciclo, nomes[op], a, b,
           reg[16], reg[17], reg[18], reg[19], reg[20],
           reg[21], reg[22], reg[23], pc, zf);
}

void max_array() {
    mem[0x00] = 0x05; mem[0x01] = 0x10; mem[0x02] = 0x0C;
    mem[0x03] = 0x05; mem[0x04] = 0x11; mem[0x05] = 0x2D;
    mem[0x06] = 0x05; mem[0x07] = 0x12; mem[0x08] = 0x07;
    mem[0x09] = 0x05; mem[0x0A] = 0x13; mem[0x0B] = 0x59;
    mem[0x0C] = 0x05; mem[0x0D] = 0x14; mem[0x0E] = 0x17;
    mem[0x0F] = 0x05; mem[0x10] = 0x15; mem[0x11] = 0x38;
    mem[0x12] = 0x05; mem[0x13] = 0x16; mem[0x14] = 0x03;
    mem[0x15] = 0x05; mem[0x16] = 0x17; mem[0x17] = 0x43;
    mem[0x18] = 0x0A; mem[0x19] = 0x00; mem[0x1A] = 0x00;
}

int main() {
    max_array();

    while (running && pc < 256) {
        uint8_t op, a, b;
        ciclo++;
        fetch(&op, &a, &b);
        decode_execute(op, a, b);
        trace(op, a, b);
    }
    return 0;
}