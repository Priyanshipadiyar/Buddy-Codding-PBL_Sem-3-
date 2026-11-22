#!/usr/bin/env python3
"""Self-contained verification for Buddy Coding Q01-Q35.
Usage: python test_cases/verify_dataset.py [repository_root]
Requires gcc and g++ on PATH; does not mutate project files.
"""
import json
import os
import re
import sys
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(sys.argv[1] if len(sys.argv) > 1 else '.').resolve()
TIMEOUT = 0.35


def strip_comments(source):
    output=[]; i=0; mode='code'; escaped=False
    while i < len(source):
        ch=source[i]; nxt=source[i+1] if i+1<len(source) else ''
        if mode=='code':
            if ch=='/' and nxt=='/': mode='line'; i+=2; continue
            if ch=='/' and nxt=='*': mode='block'; i+=2; continue
            if ch in ('"', "'"): mode=ch; escaped=False
            output.append(ch); i+=1
        elif mode=='line':
            if ch=='\n': mode='code'; output.append('\n')
            i+=1
        elif mode=='block':
            if ch=='*' and nxt=='/': mode='code'; i+=2; continue
            if ch=='\n': output.append('\n')
            i+=1
        else:
            output.append(ch)
            if escaped: escaped=False
            elif ch=='\\': escaped=True
            elif ch==mode:mode='code'
            i+=1
    return ''.join(output)


def normalize(source):
    return ''.join(strip_comments(source).split())


def compile_program(source, executable, cpp, strict):
    cmd=['g++' if cpp else 'gcc','-std=c++17' if cpp else '-std=c11',
         '-O0','-Wall','-Wextra','-pedantic']
    if strict: cmd.append('-Werror')
    result=subprocess.run(cmd+[str(source),'-o',str(executable)],capture_output=True,text=True,timeout=20)
    return result.returncode == 0, result.stderr


def run_case(executable, case):
    try:
        completed=subprocess.run([str(executable)],input=case['input'],capture_output=True,text=True,timeout=TIMEOUT)
        return completed.returncode == 0 and completed.stdout == case['expected']
    except subprocess.TimeoutExpired:
        return False


def verify_question(q, directory):
    errors=[]
    stats={"reference_passes":0,"references":0,"wrong_code_compiled":0,"wrong_code_detected":0,"wrong_code_failed_expected_case":0}
    id=f'Q{q:02}'; p=ROOT/'database'/'problems'/f'{id}.json'
    data=json.loads(p.read_text(encoding='utf8'))
    tests=data['testCases']; categories=data['mistakes']
    if len(tests)!=6 or [x['id'] for x in tests]!=[f'T{i}' for i in range(1,7)]:
        errors.append(f'{id}: tests must be T1 through T6'); return errors, stats
    if len(categories)!=6:errors.append(f'{id}: must have six mistake categories')
    for t in tests:
        folder=ROOT/'test_cases'/id
        if (folder/f"{t['id']}_in.txt").read_text(encoding='utf8')!=t['input']:
            errors.append(f'{id} {t["id"]}: input file disagrees with JSON')
        if (folder/f"{t['id']}_out.txt").read_text(encoding='utf8')!=t['expected']:
            errors.append(f'{id} {t["id"]}: output file disagrees with JSON')
    for category in categories:
        if any(('MISTAKE:' in v or '//' in v or '(?m)' in v or '(?s)' in v or v.startswith('re:')) for v in category['detect']['anyPattern']):
            errors.append(f'{id} {category["category"]}: prohibited pattern')
        if len([k for k in category if re.fullmatch(r'hint[1-7]',k)])!=7:
            errors.append(f'{id} {category["category"]}: missing progressive hints')
    for lang,ext,cpp in [('C','c',False),('CPP','cpp',True)]:
        folder=ROOT/'test_cases'/id/lang
        programs=[(folder/f'correct.{ext}',None)]
        programs += [(folder/f'wrong_{cat["category"]}.{ext}',cat['category']) for cat in categories]
        for program,expected_cat in programs:
            if not program.is_file():errors.append(f'{program}: missing');continue
            code=program.read_text(encoding='utf8'); output=directory/f'{id}_{lang}_{expected_cat or "correct"}'
            compiles,stderr=compile_program(program,output,cpp,expected_cat is None)
            if not compiles:errors.append(f'{program}: compiler failed: {stderr[:250]}');continue
            passes=[t['id'] for t in tests if run_case(output,t)]
            failed=[t['id'] for t in tests if t['id'] not in passes]
            found=[cat['category'] for cat in categories if any(normalize(pattern) in normalize(code)
                   for pattern in cat['detect']['anyPattern']) and
                   (expected_cat is None or set(failed)&set(cat['detect']['failsAnyOf']))]
            if expected_cat is None:
                stats['references']+=1
                stats['reference_passes']+=len(passes)
                if failed:errors.append(f'{program}: reference fails {failed}')
                if found:errors.append(f'{program}: correct source matches {found}')
            else:
                stats['wrong_code_compiled']+=1
                if expected_cat not in found or len(found)!=1:
                    errors.append(f'{program}: classified as {found}; expected only {expected_cat}')
                else:stats['wrong_code_detected']+=1
                required=next(cat['detect']['failsAnyOf'] for cat in categories if cat['category']==expected_cat)
                if not (set(failed)&set(required)):
                    errors.append(f'{program}: no failure among failsAnyOf {required}')
                else:stats['wrong_code_failed_expected_case']+=1
    print(f'{id}: checked',flush=True)
    return errors, stats

def verify():
    from concurrent.futures import ThreadPoolExecutor
    errors=[]
    stats={"reference_passes":0,"references":0,"wrong_code_compiled":0,"wrong_code_detected":0,"wrong_code_failed_expected_case":0}
    with tempfile.TemporaryDirectory(prefix='buddy_dataset_check_') as td:
        directory=Path(td)
        with ThreadPoolExecutor(max_workers=8) as pool:
            for issues, counts in pool.map(lambda q: verify_question(q, directory), range(1,36)):
                errors.extend(issues)
                for key, value in counts.items(): stats[key]+=value
    print(json.dumps({'statistics':stats,'errors':errors[:30], 'error_count':len(errors)},indent=2))
    return not errors

if __name__=='__main__':sys.exit(0 if verify() else 1)
