/*
    TRABALHO PARA REC DE ORGANIZACAO DE COMPUTADORES
   
    
    THIAGO LIMA E SILVA RISSOTTO
    N.USP: 9806641

*/


#include <stdio.h>
#include <stdlib.h>
#define not_implemented() fprintf(stderr, "Not implemented\n"); exit(EXIT_FAILURE)
#include "mask.h"


 /*DEFINICAO DE TODOS 
   OS MEM'S QUE SERAO USADOS*/
#define MEM0   0x9408   // 1001 0100 0000 1000
#define MEM1   0x0018   // 0000 0000 0001 1000
#define MEM2   0x0014   // 0000 0000 0001 0100
#define MEM3   0x1800   // 0001 1000 0000 0000
#define MEM4   0x4002   // 0100 0000 0000 0010
#define MEM5   0x0802   // 0000 1000 0000 0010
#define MEM6   0x0044   // 0000 0000 0100 0100
#define MEM7   0x0003   // 0000 0000 0000 0011
#define MEM8   0x02a4   // 0000 0010 1010 0100
#define MEM9   0x0488   // 0000 0100 1000 1000
#define MEM10  0x0002   // 0000 0000 0000 0010
#define MEM11  0x0082   // 0000 0000 1000 0010
#define MEM12  0x02e4   // 0000 0010 1110 0100



// Variáveis globais utilizadas para definir o bit zero e o overflow da ULA.
char zero;
char overflow;

/*
    Função auxiliar que recebe como parâmetros os bits de controle(SC) e os bits da instrução(IR) e
    retorna os bits de entrada de controle da ULA indicando a operação a ser efetuada.
*/
void controle_ula(short int sc, int IR, char *alu_op)
{
    // Inicializa variáveis auxiliares que representam os bits de opALU.
    short int opALU0, opALU1;

    // Separa os bits de opALU que estão nos bits de controle(SC).
    opALU0 = sc & separa_ALUOp0;
    opALU1 = sc & separa_ALUOp1;

    // Shifts a direita utilizados para obter os valores correspondentes a 0 ou 1.
    opALU0 = opALU0 >> 5;
    opALU1 = opALU1 >> 6;

    // Verifica se os bits de opALU são 00 e inicializa os bits de operação para soma.
    if(opALU0 == (short int)0 && opALU1 == (short int)0){
        *alu_op = *alu_op | ativa_soma;
    }

    // Verifica se o bit opALU0 é 1 e inicializa os bits de operação para subtração.
    else if(opALU0 == (short int)1){
        *alu_op = *alu_op | ativa_subtracao;
    }

    // Verifica se o bit opALU1 é 1.
    else if(opALU1 == (short int)1){

        // Inicializa uma variável auxiliar com os bits do campo função da respectiva instrução.
        int funct;
        funct = IR & separa_cfuncao;

        // Caso os bits do campo função representem soma, inicializa os bits de operação para soma.
        if(funct == funct_add){
            *alu_op = *alu_op | ativa_soma;
        }

        // Caso os bits do campo função representem subtração, inicializa os bits de operação para subtração.
        else if(funct == funct_subtract){
            *alu_op = *alu_op | ativa_subtracao;
        }

        // Caso os bits do campo função representem and, inicializa os bits de operação para and.
        else if(funct == funct_and){
            *alu_op = *alu_op | ativa_and;
        }

        // Caso os bits do campo função representem or, inicializa os bits de operação para or.
        else if(funct == funct_or){
            *alu_op = *alu_op | ativa_or;
        }

        // Caso os bits do campo função representem slt, inicializa os bits de operação para slt.
        else if(funct == funct_slt){
            *alu_op = *alu_op | ativa_slt;
        }
    }
}

int alu( int a, int b, char alu_op, int *result_alu, char *zero, char *overflow)
{
    // Verifica se os bits de operação represetam soma.
    if(alu_op == (char)ativa_soma){

        // Resultado da ULA recebe a + b;
        *result_alu = a + b;
    }

    // Verifica se os bits de operação represetam subtração.
    else if(alu_op == (char)ativa_subtracao){

        // Resultado da ULA recebe a - b;
        *result_alu = a - b;
    }

    // Verifica se os bits de operação represetam and.
    else if(alu_op == (char)ativa_and){

        // Resultado da ULA recebe a & b;
        *result_alu = a & b;
    }

    // Verifica se os bits de operação represetam or.
    else if(alu_op == (char)ativa_or){

        // Resultado da ULA recebe a | b;
        *result_alu = a | b;
    }

    // Verifica se os bits de operação represetam slt.
    else if(alu_op == (char)ativa_slt){

        // Caso a seja menor que b, resultado da ULA recebe 1.
        if(a < b){
            *result_alu = 1;
        }

        // Caso contrário, resultado da ULA recebe 0.
        else{
            *result_alu = 0;
        }
    }

    // Verifica se o resultado da ULA é 0. Se sim, inicializa o bit zero como 1.
    if(*result_alu == 0){
        *zero = 0;
        *zero = *zero | ativa_bit_zero;
    }

    // Caso contrário, inicializa o bit zero como 0.
    else{
        *zero = 0;
        *zero = *zero & desativa_bit_zero;
    }

    return 0;
}


void control_unit(int IR, short int *sc)
{
    // Verifica se é o início e inicializa os bits de controle(SC) para o estado 0.
    if(*sc == (short int) 0){
        *sc = *sc | ativa_MemRead;
        *sc = *sc & desativa_ALUSrcA;
        *sc = *sc & desativa_IorD;
        *sc = *sc | ativa_IRWrite;
        *sc = *sc | ativa_ALUSrcB0;
        *sc = *sc & desativa_ALUSrcB1;
        *sc = *sc & desativa_ALUOp0;
        *sc = *sc & desativa_ALUOp1;
        *sc = *sc | ativa_PCWrite;
        *sc = *sc & desativa_PCSource0;
        *sc = *sc & desativa_PCSource1;
    }

    // Verifica se está no estado 0 e inicializa os bits de controle para o estado 1.
    else if(*sc == (short int)estado0){
        *sc = 0;
        *sc = *sc & desativa_ALUSrcA;
        *sc = *sc | ativa_ALUSrcB0;
        *sc = *sc | ativa_ALUSrcB1;
        *sc = *sc & desativa_ALUOp0;
        *sc = *sc & desativa_ALUOp1;
    }

    // Verifica se está no estado 1 para decidir qual é o próximo estado.
    else if(*sc == (short int)estado1){

        // Inicializa uma variável auxiliar com os bits de opcode da respectiva instrução.
        int aux_opcode = IR & separa_cop;

        // Verifica se os bits de opcode representam uma LW ou uma SW e
        // inicializa os bits de controle para o estado 2.
        if(aux_opcode == lw || aux_opcode == sw){
            *sc = 0;
            *sc = *sc | ativa_ALUSrcA;
            *sc = *sc & desativa_ALUSrcB0;
            *sc = *sc | ativa_ALUSrcB1;
            *sc = *sc & desativa_ALUOp0;
            *sc = *sc & desativa_ALUOp1;
        }

        // Verifica se os bits de opcode representam uma tipo R e
        // inicializa os bits de controle para o estado 6.
        else if(aux_opcode == tipoR){
            *sc = 0;
            *sc = *sc | ativa_ALUSrcA;
            *sc = *sc & desativa_ALUSrcB0;
            *sc = *sc & desativa_ALUSrcB1;
            *sc = *sc & desativa_ALUOp0;
            *sc = *sc | ativa_ALUOp1;
        }

        // Verifica se os bits de opcode representam uma BEQ e
        // inicializa os bits de controle para o estado 8.
        else if(aux_opcode == beq){
            *sc = 0;
            *sc = *sc | ativa_ALUSrcA;
            *sc = *sc & desativa_ALUSrcB0;
            *sc = *sc & desativa_ALUSrcB1;
            *sc = *sc | ativa_ALUOp0;
            *sc = *sc & desativa_ALUOp1;
            *sc = *sc | ativa_PCWriteCond;
            *sc = *sc | ativa_PCSource0;
            *sc = *sc & desativa_PCSource1;
        }

        // Verifica se os bits de opcode representam uma JUMP e
        // inicializa os bits de controle para o estado 9.
        else if(aux_opcode == jump) {
            *sc = 0;
            *sc = *sc | ativa_PCWrite;
            *sc = *sc & desativa_PCSource0;
            *sc = *sc | ativa_PCSource1;
        }
    }

    // Verifica se está no estado 2 para decidir qual é o próximo estado.
    else if(*sc == (short int)estado2){

        // Inicializa uma variável auxiliar com os bits de opcode da respectiva instrução.
        int aux_opcode = IR & separa_cop;
        //aux_opcode >> 20;

        // Verifica se os bits de opcode representam uma LW e
        // inicializa os bits de controle para o estado 3.
        if(aux_opcode == lw){
            *sc = 0;
            *sc = *sc | ativa_MemRead;
            *sc = *sc | ativa_IorD;
        }

        // Verifica se os bits de opcode representam uma SW e
        // inicializa os bits de controle para o estado 5.
        else if(aux_opcode == sw){
            *sc = 0;
            *sc = *sc | ativa_MemWrite;
            *sc = *sc | ativa_IorD;
        }
    }

    // Verifica se está no estado 3 e inicializa os bits de controle para o estado 4.
    else if(*sc == (short int)estado3){
        *sc = 0;
        *sc = *sc & desativa_RegDst;
        *sc = *sc | ativa_RegWrite;
        *sc = *sc | ativa_MemtoReg;
    }

    // Verifica se está no estado 6 e inicializa os bits de controle para o estado 7.
    else if(*sc == (short int)estado6){
        *sc = 0;
        *sc = *sc | ativa_RegDst;
        *sc = *sc | ativa_RegWrite;
        *sc = *sc & desativa_MemtoReg;
    }

    // Verifica se está no estado 4, 5, 7, 8 ou 9 e inicializa os bits de controle para o estado 0.
    else if(*sc == (short int)estado4 || *sc == (short int)estado5 || *sc == (short int)estado7 || *sc == (short int)estado8 || *sc == (short int)estado9){
        *sc = 0;
        *sc = *sc | ativa_MemRead;
        *sc = *sc & desativa_ALUSrcA;
        *sc = *sc & desativa_IorD;
        *sc = *sc | ativa_IRWrite;
        *sc = *sc | ativa_ALUSrcB0;
        *sc = *sc & desativa_ALUSrcB1;
        *sc = *sc & desativa_ALUOp0;
        *sc = *sc & desativa_ALUOp1;
        *sc = *sc | ativa_PCWrite;
        *sc = *sc & desativa_PCSource0;
        *sc = *sc & desativa_PCSource1;
    }
}


void instruction_fetch(short int sc, int PC, int ALUOUT, int IR, int* PCnew, int* IRnew, int* MDRnew)
{
    // Verifica se está no estado 0(busca de instrução).
    if(sc == (short int)estado0){

        // Inicializa uma variável auxiliar que representa os bits de operação da ULA.
        char alu_op = 0;

        // Armazena a instrução apontada por PC na memória em IR.
        *IRnew = memory[PC];

        // Condição de parada do programa.
        if(*IRnew == 0){
            loop = 0;
        }

        // Função que retorna os bits de operação da ULA com base no SC e no IR.
        controle_ula(sc, IR, &alu_op);

        // Função que retorna o endereço da próxima instrução sequencial(PC+4).
        alu(PC, 1, alu_op, PCnew, &zero, &overflow);

    }
}


void decode_register(short int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew)
{
    // Verifica se está no estado 1(busca dos registradores e decodificação da instrução).
    if(sc == (short int)estado1){

        // Inicializa uma variável auxiliar que representa o número do registrador A.
        // Shifts a direita utilizados para obter o valor correspondente entre 0 a 31.
        int regA = IR & separa_rs;
        regA = regA >> 21;

        // Inicializa uma variável auxiliar que representa o número do registrador B.
        // Shifts a direita utilizados para obter o valor correspondente entre 0 a 31.
        int regB = IR & separa_rt;
        regB = regB >> 16;

        // Armazena em A e B os valores lidos do banco de registradores representados por regA e regB.
        *Anew = reg[regA];
        *Bnew = reg[regB];

        // Inicializa uma variável auxiliar que representa o offset(16 bits menos significativos).
        int imediato = IR & separa_imediato;
        //imediato = imediato << 2;

        // Inicializa uma variável auxiliar que representa os bits de operação da ULA.
        char alu_op = 0;

        // Função que retorna os bits de operação da ULA com base no SC e no IR.
        controle_ula(sc, IR, &alu_op);

        // Função que retorna o endereço de destino do desvio com a ULA.
        alu(PC, imediato, alu_op, ALUOUTnew, &zero, &overflow);
    }
}


void exec_calc_end_branch(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew)
{
    // Verifica se está no estado 2(cálculo do endereço de memória).
    if(sc == (short int)estado2){

        // Inicializa uma variável auxiliar que representa os bits de operação da ULA.
        char alu_op = 0;

        // Função que retorna os bits de operação da ULA com base no SC e no IR.
        controle_ula(sc, IR, &alu_op);

        // Inicializa uma variável auxiliar que representa o offset(16 bits menos significativos).
        int imediato = IR & separa_imediato;
        //imediato = imediato << 2;

        // Função que retorna o endereço de memória com a ULA.
        alu(A, imediato, alu_op, ALUOUTnew, &zero, &overflow);
    }

    // Verifica se está no estado 6(execução).
    else if(sc == (short int)estado6){

        // Inicializa uma variável auxiliar que representa os bits de operação da ULA.
        char alu_op = 0;

        // Função que retorna os bits de operação da ULA com base no SC e no IR.
        controle_ula(sc, IR, &alu_op);

        // Função que retorna o resultado da operação especificada pelo campo função com os valores lidos A e B com a ULA.
        alu(A, B, alu_op, ALUOUTnew, &zero, &overflow);
    }

    //Verifica se está no estado 8(conclusão do branch).
    else if(sc == (short int)estado8){

        // Inicializa uma variável auxiliar que representa os bits de operação da ULA.
        char alu_op = 0;

        // Função que retorna os bits de operação da ULA com base no SC e no IR.
        controle_ula(sc, IR, &alu_op);

        // Função que retorna o resultado da operação subtração com os valores lidos A e B com a ULA.
        alu(A, B, alu_op, ALUOUTnew, &zero, &overflow);

        // Caso o bit zero seja 1(valores de A e B são iguais), PC recebe o endereço de destino do desvio.
        if(zero){
            *PCnew = ALUOUT;
        }
    }

    // Verifica se está no estado 9(conclusão do jump).
    else if(sc == (short int)estado9){

        //Inicializa uma variável auxiliar que recebe os 4 mais significativos de PC.
        int pc4bits = PC & separa_4bits_PC;

        // Inicializa uma variável auxiliar que recebe o 26 bits menos signifcativos de IR.
        int calc_jump = IR & separa_endereco_jump;

        // Shifts a esquerda para obter os 28 bits correspondentes.
        calc_jump = calc_jump << 2;

        // PC recebe concatenação dos 4 bits mais significativos de PC e os 28 bits calculados anteriormente.
        *PCnew = pc4bits | calc_jump;
    }
}


void write_r_access_memory(short int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew)
{
    // Verifica se está no estado 3(acesso à memória).
    if(sc == (short int)estado3){

        // MDR recebe os dados lidos da memória com endereço indicado por ALUOUT.
        *MDRnew = memory[ALUOUT];
    }

    // Verifica se está no estado 5(acesso à memória).
    else if(sc == (short int)estado5){

        // Memória recebe os dados do operando de origem salvo em B.
        memory[ALUOUT] = B;
    }

    // Verifica se está no estado 7(conclusão do tipo R).
    else if(sc == (short int)estado7){

        // Inicializa uma variável auxiliar que representa o número do registrador de destino (rd).
        // Shifts a direita utilizados para obter o valor correspondente entre 0 a 31.
        int regDst = IR & separa_rd;
        regDst = regDst >> 11;

        // Registrador de destino recebe os dados calculados armazenados em ALUOUT.
        reg[regDst] = ALUOUT;
    }
}


void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT)
{
    // Verifica se está no estado 4(conclusão da leitura de memória).
    if(sc == (short int)estado4){

        // Inicializa uma variável auxiliar que representa o número do registrador de destino rt.
        // Shifts a direita utilizados para obter o valor correspondente entre 0 a 31.
        int regB = IR & separa_rt;
        regB = regB >> 16;

        // Registrador rt recebe os dados da memória armazenados em MDR.
        reg[regB] = MDR;
    }
}


// Unidade de controle da CPU para funcionamento das instruções
void control_cpu(int IR, short int *sc)
{
    int rt;
    int op;


    if(IR == -1)
    {
        *sc = MEM0;
    }

    else
    {
        //Separa o codigo de operação
        op = ((IR & separa_cop) >> 26);
        rt = ((IR & separa_rt) >> 16);

        if(*sc == ((short int)MEM0))
        {
            *sc = MEM1;
        }

        // Cada instrução
        else
        {
            // Instruções tipo R
            if (op==0x00)
            {
                if (*sc==MEM1) 
                    *sc = MEM6;
                else if (*sc==MEM6)
                    *sc = MEM7;
                else if (*sc==MEM7) 
                    *sc = MEM10;
            }

            // Instrução Add Immediate
            if (op==0x08)
            {
                if(*sc==MEM1) 
                    *sc = MEM2;
                else if(*sc==MEM2) 
                    *sc = MEM10;
                else if (*sc==MEM10) 
                    *sc = MEM0;
            }

            // Instrução Jump And Link
            if (op==0x03)
            {
                if (*sc==MEM1) 
                    *sc = MEM9;
                else if (*sc==MEM9) 
                    *sc = MEM11;
                else if (*sc==MEM11) 
                    *sc = MEM0;
            }

            // Instrução Branch on Less Than Zero
            if (op==0x01)
            {
                if (rt == 0x00)
                {
                    if (*sc==MEM1) 
                        *sc = MEM12;
                    else if (*sc==MEM12) 
                        *sc = MEM0;
                }
                else if (rt == 0x02)
                {
                    if (*sc==MEM1) 
                        *sc = MEM12;
                    else if (*sc==MEM12) 
                        *sc = MEM11;
                    else if (*sc==MEM11) 
                        *sc = MEM0;
                }
            }
        }
    }
}

// Busca a proxima instrucao a ser executada
void instruction_next(short int sc, int PC, int ALUOUT, int IR, int* PCnew, int* IRnew, int* MDRnew)
{
    char overflow;
    char zero; 

    char alu_op;

    if(sc == ((short int)MEM0))
    {
        *IRnew = memory[PC];
        

        // Incrementa no PC
        alu(PC, 1, alu_op, &ALUOUT, &zero, &overflow);

        *PCnew = ALUOUT;
        *MDRnew = memory[PC];

        if(*IRnew == 0)
        {
            loop = 0;
            return;
        }
    }

    return;

}