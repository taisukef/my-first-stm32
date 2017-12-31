#!/usr/bin/python
# coding=utf-8

import sys, os, platform, subprocess

try:
################################################################################
# ボーレート (ST保証のボーレート: 1200 - 115200 bps)
	baudrate = "230400"

# シリアルポート
	if platform.system() == "Windows":
		port = "com4"
	elif platform.system() == "Linux":
		port = "/dev/ttyUSB0"
	else: # 他のOS
		port = "/dev/ttyUSB0"

# 書き込むモトローラS形式ファイル
	mot_files = os.path.join("static", "build", "static.mot")
	mot_files += " " + os.path.join("main", "build", "stm32.mot")

# ターミナルソフト
	if platform.system() == "Windows":
		termsoft = "C:/PROGRA~1/teraterm/ttermpro.exe"
#		termsoft = "C:/PROGRA~2/teraterm/ttermpro.exe" # 64bit-Windows
	elif platform.system() == "Linux":
		termsoft = "kermit"
	else: # 他のOS
		termsoft = "kermit"
################################################################################

	# このスクリプトのあるディレクトリへ移動する
	this_dir = os.path.dirname(os.path.realpath(__file__))
	os.chdir(this_dir)

	# 書き込み
	command = os.path.join("stm32writer", "stm32writer.py")
	command += " --port=%s" % port
	command += " --baudrate=%s" % baudrate
	command += " --show-erase-page"
	command += " --compare"
	command += " --go"
	command += " --time"
	command += " " + mot_files
	print command
	print
	p = subprocess.Popen(command, shell=True) # コマンドの実行
	ret = p.wait()
	print
	if ret: sys.exit(1)


	# ターミナルソフトの呼び出し
	print termsoft
	print
	p = subprocess.Popen(termsoft, shell=True) # コマンドの実行
	ret  = p.wait()
	print
	if ret: sys.exit(1)


except KeyboardInterrupt:
	sys.exit(1)
finally:
	# Windows の場合は入力受付状態にしてプロンプトを閉じないようにする
	if platform.system() == "Windows": raw_input("press enter to exit")

