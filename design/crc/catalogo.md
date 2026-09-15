# CatalogoItens e CatalogoMonstros

Coleções carregadas dos arquivos em `data/` na inicialização.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Carregar um arquivo texto, ignorando comentários e linhas vazias | `Item` / `Monstro` |
| Interromper a carga apontando arquivo e linha quando encontra dado inválido | `ArquivoInvalido` |
| Recusar entradas duplicadas pelo nome | `Dado` |
| Entregar cópias novas de um item ou monstro pelo nome | `Loja` |
| Listar todos os itens ou filtrar por tipo | `Jogo` |
| Listar monstros até um nível máximo | |
| Sortear um monstro compatível com o nível do herói, com fallback para os mais fracos | |
