# MiniEP08 - Simulando Sistemas Distribuídos

Numa breve descrição, esse mini ep visa o aprendizado sobre a bancada de testes Maelstrom,
conceitos básicos de Go e sistemas distribuídos.

O enunciado pode ser acessado [aqui](https://ablaze-bush-9be.notion.site/Enunciado-do-Mini-EP8-1382f3f8d8e98008b7a3f31d8933e808)

## Dependências

É necessário a instalação do GoLang

```bash
sudo add-apt-repository ppa:longsleep/golang-backports
sudo apt update
sudo apt install golang-go
```

A versão utilizada foi

> go version go1.23.4 linux/amd64

## Execução

Para a parte 1 do problema:

```bash
~Parte_1/maelstrom-echo$ go get github.com/jepsen-io/maelstrom/demo/go
~Parte_1/maelstrom-echo$ go install .
~Parte_1/maelstrom$ ./maelstrom test -w echo --bin ~/go/bin/maelstrom-echo --node-count <Node-count> --time-limit <Time-limit>
```

Para a parte 2 do problema:

```bash
~Parte_2/maelstrom-unique-ids$ go get github.com/jepsen-io/maelstrom/demo/go
~Parte_2/maelstrom-unique-ids$ go install .
~Parte_2/maelstrom$ ./maelstrom test -w unique-ids --bin ~/go/bin/maelstrom-unique-ids --time-limit <Time-limit> --rate <Rate> --node-count <Node-count>
```

Para a parte 3 do problema:

```bash
~Parte_3/maelstrom-unique-ids$ go get github.com/jepsen-io/maelstrom/demo/go
~Parte_3/maelstrom-unique-ids$ go install .
~Parte_3/maelstrom$ ./maelstrom test -w unique-ids --bin ~/go/bin/maelstrom-unique-ids --time-limit <Time-limit> --rate <Rate> --node-count <Node-count> --availability total --nemesis partition
```

## Resultados

Podem ser obtidos em *MAC0219___Relatório_Mini_EP_8-2-1.pdf*

## Atualizações

Até um certo tempo, mantive os diretórios organizados conforme especificado. Contudo, isso enviesou as linguagens do mini ep. Assim, recomendo baixar as dependências conforme o enunciado do mini ep para conseguir rodar os programas criados.
