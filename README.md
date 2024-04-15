<!-- Aqui va un img (header) -->

<div align="center">
	<img src="https://img.shields.io/badge/status-in progress-success?color=%2300599C&style=flat" />
	<img src="https://img.shields.io/badge/score-MARK%20%2F%20100-success?color=%2300599C&style=flat" />
	<img src="https://img.shields.io/badge/evaluated-DD%20%2F%20MM%20%2F%202024-success?color=%2300599C&style=flat" />
	<img src='https://img.shields.io/badge/Málaga-00599C?style=flat&logo=42&logoColor=white'/>
</div>

---

<p align="center">
	<a href="#project-overview">About</a> •
	<a href="#how-to-use">How to use</a> •
	<a href="#features">Features</a> •
	<a href="#mandatory">Mandatory</a> •
	<a href="#license">License</a>
</p>


# Webserv

This project is about writing your ow HTTP server. You will be able to test it with an actual browser. HTTP is one of the most used protocols on the internet. Knowing its arcane will be useful, even if you won’t be working on a website.

## Project Overview

The objective of this project is to create a fully functional HTTP web server.

>[Click](./docs/en.subject.pdf) to view the project `subject`.
>
>[Click](https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7) to view a project guide.

## How to Use
### Compile

Copy and paste this code to download and compile the project.

```bash
git clone https://github.com/XdarksyderX/webserv.git && cd webserv && make && make clean
```

### Usage

Upon execution, we specify the configuration file located in **config/** with the **'.config'** extension as a parameter.

Example:

```bash
./webserv config/webserv.config
```
<!-- detallar el uso -->

## Features
### ✅ It supports:
* [GET, POST, DELETE] HTTP methods
* A configuration file given as an argument (examples of configurations in 'config' directory)

### ❌ It doesn't support:
* other HTTP methods

<!-- ## Configuration -->
<!-- ## Contribution -->
## License

This work is published under the terms of [42 Unlicense](https://github.com/XdarksyderX/webserv/blob/main/LICENSE).
