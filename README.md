# EA876 - Trabalho 2

Este trabalho tem como objetivo determinar se a aplicação de um filtro de imagem é mais rápida se ocorrer em uma única linha de execução, em múltiplas threads ou em múltiplos processos.

## Descrição

Ao ser executado, o programa espera a inserção por parte do usuário de algumas informações de execução, a saber: a variação a ser utilizada (None, Thread ou Process), a quantidade de execuções a serem realizadas e o parâmetro N do filtro de imagem.

## Utilização

### Dependências

Para o correto funcionamento do programa, é necessário possuir make e a biblioteca FreeImage instalados.

### Executando o programa

A execução do programa é bem simples. Para executá-lo, basta digitar o comando 'make run' enquanto na pasta root do programa.

### Durante a execução

Depois de digitar o comando 'make run' no terminal, o programa será executado. O usuário deve então simplesmente entrar com alguns parâmetros de execução, como a variação a ser utilizada (None, Thread ou Process), o número de execuções e o parâmetro N do filtro blur. A imagem utilizada para filtragem deve ser selecionada através do código fonte.

### Resultado

O programa irá gerar as imagens filtradas no diretório 'data/'. Além disso, o programa printa o tempo de execução de cada execução na tela do usuário.
O diretório 'doc/' contém as figuras geradas pelo código 'doc/plot.py', que processa o tempo de execução do código 'src/main.c'. A figura 'Resultados.pdf' possui todas as imagens em um só arquivo, como solicitado. A execução do código 'doc/plot.py' é por conta do usuário.

## Autor

João Antônio Gória Silva -- jgoriasilva@gmail.com
