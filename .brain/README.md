# .brain — memória compartilhada do TP

Contexto comum para os assistentes de código dos três integrantes. As regras gerais estão no [AGENTS.md](../AGENTS.md) da raiz.

| Arquivo | Para quê |
|---|---|
| `dialogo_ias.md` | Log cronológico (só acrescentar no fim). Cada entrada termina com "Próxima IA / Handoff" |
| `pendencias/PEND-XX` | Uma tarefa por arquivo, com responsável e prazo no frontmatter |
| `pendencias.md` | Sumário gerado por `python .brain/scripts/manage_brain.py sync` (não editar à mão) |
| `decisoes/` | Decisões de projeto que valem para todos |

Ao terminar um bloco de trabalho: entrada nova no diálogo, `status` da pendência atualizado, `sync`, commit junto com o trabalho.
