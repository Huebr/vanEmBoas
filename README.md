# vanEmBoas
Implementação Van Em Boas usando SimpleHashingTable para disciplina de Estrutura de Dados Avançados UFC 2020.1. 

#Ambiente

Este projeto foi desenvolvido usando Visual Studio 2017, com perfil x64/Debug.

Para compilar no linux, basta ir na pasta raiz: g++ -O3 Hashing.cpp -o hashing
Modo de usar

./hashing input.txt output.txt

input.txt: Arquivo de Entrada formato do projeto. 
output.txt: Arquivo de Saida formato do projeto. Deve ser criado antes de chamar o programa.

#Projeto

Imprementar uma van Emde Boas  que utiliza espaço linear.

Devem ser implementadas as seguintes operações:

Incluir(inteiro x): inclui o inteiro de 64 bits x (manter apenas uma cópia de cada inteiro)
Remover(Inteiro x): remover o inteiro x (não fazer nada caso tal ocorrência não esteja na tabela)
Sucessor(Inteiro x): retorna o sucessor de x na estrutura (o menor valor dentro da estrutura que é estritamente maior que x ou NULO se tal valor não existir)
Predecessor(Inteiro x): retorna o predecessor de x na estrutura (o maior valor dentro da estrutura que é estritamente menor que x ou NULO se tal valor não existir)

ENTRADA:
Vocês precisam ler um arquivo de texto que será dado como entrada para realizar um conjunto de operações.
Cada linha deste arquivo pode conter as seguintes informações

INC:x (incluir o inteiro x)
REM:x (remover o inteiro x)
SUC:x (sucessor do inteiro x)
PRE:x (predecessor do inteiro x)

Exemplo de um arquivo de entrada:

INC:10
INC:20
INC:30
INC:37
SUC:10
SUC:15
PRE:42
REM: 37
PRE:42


SAÍDA:
Você deve gerar um arquivo de texto como saída 
As linhas do arquivo de entrada serão copiadas no arquivo de saída.
As operações de Precedessor e Sucessor serão acompanhadas do resultado na linha imediatamente seguinte.
Será adicionado um espaço antes da próxima linha de entrada.

Exemplo de saída:

INC:10

INC:20

INC:30

INC:37

SUC:10
20

SUC:15
20

PRE:42
37

REM: 37

PRE:42
30
