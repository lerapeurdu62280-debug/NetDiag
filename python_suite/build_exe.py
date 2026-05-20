#!/usr/bin/env python3
"""
Build Python suite to standalone .EXE files
Requires: pip install pyinstaller
"""

import os
import sys
import subprocess
from pathlib import Path

def build_exe(script_name: str, output_name: str, console: bool = True):
    """
    Build Python script to .EXE using PyInstaller
    """
    print(f"\n[BUILD] Compiling {script_name} -> {output_name}.exe")
    print("="*60)
    
    cmd = [
        'pyinstaller',
        '--onefile',  # Single file
        '--windowed' if not console else '--console',  # Console window
        '--icon=NONE',  # No icon for now
        f'--name={output_name}',
        f'--distpath=./dist',
        f'--buildpath=./build',
        script_name
    ]
    
    try:
        result = subprocess.run(cmd, check=True, capture_output=True, text=True)
        print(f"[SUCCESS] {output_name}.exe created successfully!")
        print(f"Location: ./dist/{output_name}.exe")
        return True
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Build failed: {e}")
        print(e.stderr)
        return False

def main():
    print("""
    ╔════════════════════════════════════════════════════════╗
    ║  NetworkDiagnostic Suite - EXE Builder                 ║
    ║  Converts Python scripts to standalone Windows EXE     ║
    ╚════════════════════════════════════════════════════════╝
    """)
    
    # Check PyInstaller installation
    try:
        subprocess.run(['pyinstaller', '--version'], capture_output=True, check=True)
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("[ERROR] PyInstaller not installed")
        print("[INFO] Install: pip install pyinstaller")
        return
    
    # Build scripts
    scripts = [
        ('1_NetworkDiagnostic.py', 'NetworkDiagnostic', True),
        ('2_PerformanceAnalyzer.py', 'PerformanceAnalyzer', True),
        ('3_TrafficAnalyzer.py', 'TrafficAnalyzer', True),
        ('4_SecurityMonitor.py', 'SecurityMonitor', True),
    ]
    
    results = []
    
    for script, output, console in scripts:
        if not os.path.exists(script):
            print(f"[WARNING] {script} not found. Skipping...")
            continue
        
        success = build_exe(script, output, console)
        results.append((output, success))
    
    # Summary
    print("\n" + "="*60)
    print("[SUMMARY]")
    print("="*60)
    
    success_count = sum(1 for _, success in results if success)
    
    for output, success in results:
        status = "✓ SUCCESS" if success else "✗ FAILED"
        print(f"  {status}: {output}.exe")
    
    print(f"\nTotal: {success_count}/{len(results)} builds successful")
    
    if success_count > 0:
        print("\n[INFO] EXE files ready in ./dist/ directory")
        print("[INFO] Each .exe is standalone - no Python installation needed!")

if __name__ == '__main__':
    main()
