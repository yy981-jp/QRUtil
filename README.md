# QRUtil

**QRUtil (QRCode Util)** は、2次元コードの読み取り・生成を行う小さなコマンドラインツールです。

C++23 で実装されており、バーコード処理には [ZXing-C++](https://github.com/zxing-cpp/zxing-cpp) を使用しています。

QRコード以外の2次元コードにも対応しています。

## 機能

- 画像ファイルから2次元コードを読み取り
- 文字列やファイルから2次元コードを生成
- 生成したコードをターミナルへ直接表示
- PNG / JPEG / SVG / テキスト形式で保存
- 読み取ったコードの形式、位置、回転、鏡像、反転、ECI、識別子などの詳細情報を表示

## 必要環境

- CMake 3.20 以上
- C++23 に対応したコンパイラ
- Ninja（推奨）
- Git

現在は Windows + MinGW-w64 を中心に開発しています。

## 依存ライブラリ

以下のライブラリを Git submodule として使用しています。

- [CLI11](https://github.com/CLIUtils/CLI11) - コマンドライン引数の解析
- [ZXing-C++](https://github.com/zxing-cpp/zxing-cpp) - バーコードの読み取り・生成
- [stb](https://github.com/nothings/stb) - 画像の読み込み、およびPNG/JPEG出力

## ビルド

submodule を含めてリポジトリを取得します。

```cmd
git clone --recursive https://github.com/yy981-jp/QRUtil.git
cd QRUtil
```

CMake と Ninja を使ってビルドします。

```cmd
cmake -S . -B build -G Ninja
cmake --build build
```

実行ファイルは以下に生成されます。

```text
build/qr.exe
```

## 使い方

QRUtil はサブコマンド方式で動作します。

```text
qr <サブコマンド> <対象> [オプション]
```

### 2次元コードを生成する

文字列から2次元コードを生成します。

```cmd
qr w "https://example.com"
```

デフォルトでは `code.png` として保存されます。

ファイルの内容から生成する場合は `-f` / `--file` を使用します。

```cmd
qr w message.txt -f
```

`-t` / `--text` と `-f` / `--file` で入力モードを指定できます。どちらも指定しない場合はテキストモードとして扱われます。

出力先を指定することもできます。

```cmd
qr w "Hello, world!" -o hello.svg
```

対応している出力形式は以下の通りです。

| 拡張子 | 形式 |
| --- | --- |
| `.png` | PNG画像 |
| `.jpg` | JPEG画像 |
| `.jpeg` | JPEG画像 |
| `.svg` | SVG |
| `.txt` | テキスト |

出力先に拡張子を指定しなかった場合は `.png` が自動的に付加されます。

### ターミナルに直接表示する

`--terminal` を指定すると、ファイルを作成せず標準出力へ直接2次元コードを出力できます。

```cmd
qr w "Hello, world!" --terminal
```

### サイズを変更する

生成時のデフォルトサイズは `10` です。

```cmd
qr w "Hello, world!" --size 20
```

### 余白をなくす

デフォルトでは2次元コードの周囲に余白（quiet zone）が追加されます。
これを無効にするには `--no-margin` を指定します。

```cmd
qr w "Hello, world!" --no-margin
```

### 2次元コードを読み取る

画像ファイルからコードを読み取ります。

```cmd
qr r code.png
```

通常は読み取った内容だけが標準出力へ出力されます。

詳細情報を表示する場合は `-d` / `--detail` を使用します。

```cmd
qr r code.png -d
```

詳細表示では以下の情報を確認できます。

- 形式
- 内容
- 位置
- 回転
- 鏡像状態
- 反転状態
- ECI の有無
- 識別子

## コマンドリファレンス

### `qr w`

2次元コードを生成します。

```text
qr w <対象> [オプション]
```

| オプション | 説明 | デフォルト |
| --- | --- | --- |
| `-o <path>` | 出力先 | `code.png` |
| `--size <n>` | 生成サイズ | `10` |
| `--no-margin` | 周囲の余白を付けない | 無効 |
| `--terminal` | 生成したコードを標準出力へ表示 | 無効 |
| `--format <format>` | 2次元コードの形式 | QR Code |
| `-t`, `--text` | テキストモード | 自動 |
| `-f`, `--file` | ファイルモード | 自動 |

利用可能な形式名は以下の通りです。

```text
aztec-code
aztec
qr-model2
qr
pdf417
itf
datamatrix
codabar
upc-e
upc-a
code128
ean-13
code39-standard
code93
ean-8
code39
```

### `qr r`

画像ファイルから2次元コードを読み取ります。

```text
qr r <画像> [オプション]
```

| オプション | 説明 |
| --- | --- |
| `-d`, `--detail` | 2次元コードの詳細情報も表示 |

## ライセンス

QRUtil は MIT License のもとで公開されています。詳細は [LICENSE](LICENSE) を参照してください。
