# Algoritmo de Huffman em C

Este é um projeto que implementa o algoritmo de compressão de dados de Huffman em linguagem C. O algoritmo de Huffman é um método de compressão sem perdas que utiliza a frequência de ocorrência dos símbolos no conjunto de dados para criar códigos de comprimento variável para cada símbolo. Ele é amplamente utilizado em aplicações de compressão de dados, como compactação de arquivos e transmissão de dados em redes.

## Funcionalidades

- Implementação do algoritmo de Huffman para compressão e descompressão de arquivos.
- Capacidade de comprimir e descomprimir uma variedade de tipos de arquivos, incluindo texto, imagens, áudio, entre outros.
- Interface de linha de comando simples e intuitiva para facilitar o uso.

## Como utilizar

1. Clone este repositório em sua máquina local.

   ```bash
   git clone https://github.com/seu_usuario/algoritmo-huffman-c.git
   ```
2. Compile o código-fonte utilizando um compilador C compatível com o padrão C99.

  ```bash
gcc -o huffman huffman.c
  ```

3. Para comprimir um arquivo, utilize o seguinte comando:

  ```bash
./huffman -c arquivo_origem arquivo_destino
```

4. Para descomprimir um arquivo, utilize o seguinte comando:

```bash
./huffman -d arquivo_comprimido arquivo_descomprimido
```

## Contribuições
- Contribuições são bem-vindas! Se você encontrar bugs ou deseja adicionar novos recursos, sinta-se à vontade para abrir uma issue ou enviar um pull request.

## Licença
- Este projeto está licenciado sob a Licença MIT, o que significa que você pode usá-lo livremente em seus próprios projetos, comerciais ou não.



