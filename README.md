# Webserv

## About Webserv

`Webserv` is a custom HTTP server written in C++, designed to simulate some basic functionalities of real-world web servers like Apache and Nginx. It handles HTTP requests, serves static resources, and executes server-side applications to generate dynamic content.

This project is built as part of an educational program to understand the inner workings of web servers and network programming.

## Features

- **HTTP 1.1 Compliance**: Handles basic HTTP/1.1 requests and responses.
- **Static File Serving**: Serves HTML, CSS, JS, and image files.
- **Dynamic Content**: Support for handling scripts via CGI.
- **Custom Configuration**: Uses a custom configuration file format similar to Nginx for server setup.
- **Error Handling**: Custom error pages for different HTTP error statuses.
- **Logging**: Basic request and error logging functionalities.

## Getting Started

### Prerequisites

Before you start using `webserv`, make sure you have the following installed on your system:
- C++ Compiler (GCC or Clang)
- Make
- Git (for version control)

### Installation

Follow these steps to get `webserv` running on your machine:

1. **Clone the Repository**
   ```
   git clone https://github.com/yourusername/webserv.git
   cd webserv
   ```

2. **Compile the Project**
   ```
   make
   ```

3. **Run the Server**
   ```
   ./webserv config/sample_config.conf
   ```
   Replace `config/sample_config.conf` with the path to your server configuration file.

### Configuration

`Webserv` is configured using a custom format in a `.conf` file. Here’s a basic example of what the configuration file might look like:
```
server {
    listen 8080;
    server_name localhost;

    location / {
        root /www/data;
    }

    location /images/ {
        root /www/data/images;
    }

    error_page 404 /errors/404.html;
}
```

This configuration sets up the server to listen on port 8080 and defines two locations with different document roots.

## Usage

Once the server is running, you can access it via a web browser:

```
http://localhost:8080/
```

You can also use tools like `curl` to test your server:

```
curl http://localhost:8080/
```

## Contributing

Contributions to `webserv` are welcome! Feel free to contribute by opening issues, providing updates to documentation, or submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Help

If you encounter any problems, please don't hesitate to open an issue on the project repository or contact the project maintainers directly.
