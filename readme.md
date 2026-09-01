 # MarketView Soumans

Sistema web para monitoramento e visualização de dados do mercado financeiro.

O MarketView foi desenvolvido com uma arquitetura baseada em C++ no backend e HTML, CSS e JavaScript no frontend. O sistema consulta dados de ações, disponibiliza essas informações por uma API REST e apresenta os resultados em uma interface de dashboard. Eu acabei criando, devido meus estudos com C++ e achei interessante misturar com as stacks que tenho, já que poxa, eu sou um iniciante e já começo a desenvoltura de cada aqui. 


## Tecnologias que utilizei: 

### Backend

- C++
- Crow 1.3.3
- CMake
- vcpkg
- libcurl
- Microsoft Visual C++ (MSVC)

### Frontend

- HTML5
- CSS3
- JavaScript
- Chart.js

### Fonte dos dados

- Yahoo Finance Chart API

A aplicação utiliza o endpoint público do Yahoo Finance para obter informações de mercado.

> A API do Yahoo Finance utilizada pelo projeto não é uma API oficial documentada e pode sofrer alterações ou limitações.

## Funcionalidades

O MarketView possui:

- Dashboard de mercado
- Monitoramento de ações
- Cotação atual
- Variação do preço
- Variação percentual
- Preço de abertura
- Máxima do dia
- Mínima do dia
- Volume negociado
- Fechamento anterior
- Máxima das últimas 52 semanas
- Mínima das últimas 52 semanas
- Visualização detalhada de ativos
- Gráficos utilizando Chart.js
- Gráfico de barras para comparação de preços
- Gráfico de pizza para visualização dos ativos monitorados
- Atualização automática dos dados
- Indicador de status da API

## Ações monitoradas

Atualmente o sistema acompanha:

- PETR4
- VALE3
- ITUB4
- BBDC4

Esses ativos podem ser alterados posteriormente no arquivo:

```text
frontend/app.js

Espero que você gostem! 