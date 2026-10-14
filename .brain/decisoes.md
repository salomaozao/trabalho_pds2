# Sumário Executivo de Decisões e Regras Vigentes

Este arquivo é o sumário central de decisões metodológicas e regras de negócio do projeto **TP de PDS II: RPG por turnos (trabalho_pds2)**. 

Para aprofundamento técnico, consulte os arquivos pontuados no diretório [`decisoes/`](decisoes).

---

## Tabela Resumo de Decisões

| ID | Título / Assunto | Módulo Afetado | Resumo Executivo | Detalhe |
| :--- | :--- | :--- | :--- | :---: |
| **[DEC-01](decisoes/01_modulos_tres_integrantes.md)** | **Redistribuição dos módulos para três integrantes** | `geral` | Com a saída de André (persistência) e Sidney (interface e relatórios): Salomão fica com combate e interface; Warley com entidad... | [Abrir](decisoes/01_modulos_tres_integrantes.md) |

---

## Como Adicionar ou Modificar Decisões
1. Crie ou edite o arquivo pontuado em `.brain/decisoes/XX_nome_descritivo.md` contendo o cabeçalho YAML frontmatter.
2. Registre a síntese na tabela acima ou execute:
   ```bash
   python .brain/scripts/manage_brain.py sync
   ```
