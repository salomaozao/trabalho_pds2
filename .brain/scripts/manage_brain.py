#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script de Governança e Sincronização da Memória do Projeto (.brain)
Uso:
    python .brain/scripts/manage_brain.py audit
    python .brain/scripts/manage_brain.py sync
"""

import sys
import os
import re
from pathlib import Path
from datetime import datetime

# Garante saída UTF-8 no Windows
if hasattr(sys.stdout, "reconfigure"):
    try:
        sys.stdout.reconfigure(encoding="utf-8")
    except Exception:
        pass

BRAIN_DIR = Path(__file__).resolve().parent.parent
DECISOES_DIR = BRAIN_DIR / "decisoes"
PENDENCIAS_DIR = BRAIN_DIR / "pendencias"
DECISOES_FILE = BRAIN_DIR / "decisoes.md"
PENDENCIAS_FILE = BRAIN_DIR / "pendencias.md"
DIALOGO_FILE = BRAIN_DIR / "dialogo_ias.md"

def parse_frontmatter(file_path):
    """Extrai metadados do bloco YAML frontmatter."""
    content = file_path.read_text(encoding="utf-8")
    match = re.match(r"^---\s*\n(.*?)\n---\s*\n", content, re.DOTALL)
    if not match:
        return {}, content
    
    fm_raw = match.group(1)
    body = content[match.end():]
    metadata = {}
    
    for line in fm_raw.splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        if ":" in line:
            key, val = line.split(":", 1)
            key = key.strip()
            val = val.strip()
            # Tratamento básico de listas [a, b]
            if val.startswith("[") and val.endswith("]"):
                items = [x.strip().strip("'\"") for x in val[1:-1].split(",") if x.strip()]
                metadata[key] = items
            else:
                metadata[key] = val.strip("'\"")
    return metadata, body

def extrair_resumo_body(body):
    """Tenta capturar um parágrafo conciso de resumo do corpo do texto."""
    for line in body.splitlines():
        line = line.strip()
        if line and not line.startswith("#") and not line.startswith("---"):
            # remove links markdown
            resumo = re.sub(r"\[(.*?)\]\(.*?\)", r"\1", line)
            resumo = resumo.replace("*", "").replace("`", "").strip()
            if len(resumo) > 130:
                return resumo[:127] + "..."
            return resumo
    return "Consulte o arquivo detalhado para mais informações."

def audit():
    """Verifica a integridade entre sumários e arquivos individuais."""
    print("=" * 60)
    print("AUDITORIA DO .BRAIN - INTEGRIDADE E CONSISTENCIA")
    print("=" * 60)
    
    errors = 0
    warnings = 0

    # 1. Checagem de Decisões
    dec_files = sorted(DECISOES_DIR.glob("*.md"))
    dec_sumario_text = DECISOES_FILE.read_text(encoding="utf-8") if DECISOES_FILE.exists() else ""
    print(f"\n[DECISOES] {len(dec_files)} arquivos pontuados encontrados.")
    for f in dec_files:
        meta, _ = parse_frontmatter(f)
        if not meta.get("id"):
            print(f"  [ERRO] {f.name}: Falta 'id' no frontmatter!")
            errors += 1
        rel_link = f"decisoes/{f.name}"
        if rel_link not in dec_sumario_text:
            print(f"  [AVISO] {f.name}: Nao referenciado em decisoes.md!")
            warnings += 1
        else:
            print(f"  [OK] {f.name} -> {meta.get('id', 'N/A')} ({meta.get('titulo', 'Sem titulo')})")

    # 2. Checagem de Pendências
    pend_files = sorted(PENDENCIAS_DIR.glob("*.md"))
    pend_sumario_text = PENDENCIAS_FILE.read_text(encoding="utf-8") if PENDENCIAS_FILE.exists() else ""
    print(f"\n[PENDENCIAS] {len(pend_files)} arquivos pontuados encontrados.")
    for f in pend_files:
        meta, _ = parse_frontmatter(f)
        if not meta.get("id"):
            print(f"  [ERRO] {f.name}: Falta 'id' no frontmatter!")
            errors += 1
        rel_link = f"pendencias/{f.name}"
        if rel_link not in pend_sumario_text:
            print(f"  [AVISO] {f.name}: Nao referenciado em pendencias.md!")
            warnings += 1
        else:
            status = meta.get('status', 'aberto')
            print(f"  [OK] {f.name} -> {meta.get('id', 'N/A')} [{status.upper()}] ({meta.get('titulo', 'Sem titulo')})")

    # 3. Checagem de Diálogo
    if not DIALOGO_FILE.exists():
        print("\n  [ERRO] dialogo_ias.md nao encontrado!")
        errors += 1
    else:
        print(f"\n[DIALOGO] dialogo_ias.md presente ({DIALOGO_FILE.stat().st_size} bytes).")

    print("\n" + "=" * 60)
    print(f"Resultado: {errors} erros criticos, {warnings} avisos.")
    print("=" * 60)
    return errors == 0

def sync():
    """Reescreve os sumários executivos garantindo sincronização estrita com os arquivos detalhados."""
    print("Sincronizando decisoes.md...")
    dec_files = sorted(DECISOES_DIR.glob("*.md"))
    dec_rows = []
    
    for f in dec_files:
        meta, body = parse_frontmatter(f)
        dec_id = meta.get("id", f.stem)
        titulo = meta.get("titulo", f.stem.replace("_", " ").title())
        mod = meta.get("modulo_afetado", "geral")
        if isinstance(mod, list):
            mod_str = ", ".join(f"`{m}`" for m in mod)
        else:
            mod_str = f"`{mod}`"
        
        resumo = meta.get("resumo") or extrair_resumo_body(body)
        rel_path = f"decisoes/{f.name}"
        dec_rows.append(f"| **[{dec_id}]({rel_path})** | **{titulo}** | {mod_str} | {resumo} | [Abrir]({rel_path}) |")

    dec_content = f"""# Sumário Executivo de Decisões e Regras Vigentes

Este arquivo é o sumário central de decisões metodológicas e regras de negócio do projeto **TP de PDS II: RPG por turnos (trabalho_pds2)**. 

Para aprofundamento técnico, consulte os arquivos pontuados no diretório [`decisoes/`](decisoes).

---

## Tabela Resumo de Decisões

| ID | Título / Assunto | Módulo Afetado | Resumo Executivo | Detalhe |
| :--- | :--- | :--- | :--- | :---: |
""" + "\n".join(dec_rows) + """

---

## Como Adicionar ou Modificar Decisões
1. Crie ou edite o arquivo pontuado em `.brain/decisoes/XX_nome_descritivo.md` contendo o cabeçalho YAML frontmatter.
2. Registre a síntese na tabela acima ou execute:
   ```bash
   python .brain/scripts/manage_brain.py sync
   ```
"""
    DECISOES_FILE.write_text(dec_content, encoding="utf-8")
    print(f"  [OK] decisoes.md sincronizado com {len(dec_rows)} decisoes.")

    print("\nSincronizando pendencias.md...")
    pend_files = sorted(PENDENCIAS_DIR.glob("*.md"))
    abertas_rows = []
    resolvidas_rows = []

    for f in pend_files:
        meta, body = parse_frontmatter(f)
        pend_id = meta.get("id", f.stem)
        titulo = meta.get("titulo", f.stem.replace("_", " ").title())
        resp = meta.get("responsavel", "A definir")
        crit = meta.get("criticidade", "media").capitalize()
        status = meta.get("status", "aberto").capitalize()
        resumo = meta.get("resumo") or extrair_resumo_body(body)
        rel_path = f"pendencias/{f.name}"

        if status.lower() in ["resolvido", "concluido", "fechado"]:
            resolvidas_rows.append(f"| *{pend_id}* | *{titulo}* | {resumo} | {meta.get('data_resolucao', 'N/A')} | [Abrir]({rel_path}) |")
        else:
            crit_fmt = f"**{crit}**" if crit.lower() == "alta" else crit
            abertas_rows.append(f"| **[{pend_id}]({rel_path})** | **{titulo}** | {resp} | {crit_fmt} | `{status}` | {resumo} | [Abrir]({rel_path}) |")

    pend_content = f"""# Sumário Executivo de Pendências e Backlog

Este arquivo é o inventário de pendências ativas, bloqueios e itens a monitorar no projeto **TP de PDS II: RPG por turnos (trabalho_pds2)**.

Para aprofundamento técnico, plano de ação e responsáveis de cada item, consulte os arquivos individuais no diretório [`pendencias/`](pendencias).

---

## Pendências Abertas / Em Andamento

| ID | Assunto / Pendência | Responsável | Criticidade | Status | Resumo e Ação Necessária | Detalhes |
| :--- | :--- | :--- | :---: | :---: | :--- | :---: |
""" + "\n".join(abertas_rows) + """

---

## Pendências Concluídas / Resolvidas

| ID | Assunto | Conclusão | Data Resolução | Registro |
| :--- | :--- | :--- | :---: | :---: |
""" + "\n".join(resolvidas_rows) + """

---

## Como Operar o Backlog
- Para **abrir uma nova pendência**: crie um arquivo `PEND-XX` em `pendencias/` e execute `python .brain/scripts/manage_brain.py sync`.
- Para **resolver uma pendência**: altere `status: resolvido` no frontmatter, adicione `data_resolucao: AAAA-MM-DD` e execute `python .brain/scripts/manage_brain.py sync`.
"""
    PENDENCIAS_FILE.write_text(pend_content, encoding="utf-8")
    print(f"  [OK] pendencias.md sincronizado ({len(abertas_rows)} abertas, {len(resolvidas_rows)} resolvidas).")

if __name__ == "__main__":
    action = sys.argv[1] if len(sys.argv) > 1 else "audit"
    if action == "audit":
        audit()
    elif action == "sync":
        sync()
    else:
        print(f"Comando '{action}' desconhecido. Use 'audit' ou 'sync'.")
