# 1. Usamos una imagen base estable
FROM gcc:latest

# 2. Instalamos las dependencias necesarias
RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev \
    libssl-dev \
    libjson-c-dev \
    build-essential \
    cmake \
    git \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# 3. Compilamos e instalamos cpr desde el código fuente
RUN git clone --branch 1.14.2 --depth 1 https://github.com/libcpr/cpr.git /tmp/cpr \
    && cd /tmp/cpr \
    && mkdir build && cd build \
    && cmake .. -DCPR_USE_SYSTEM_CURL=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release \
    && cmake --build . --parallel $(nproc) \
    && cmake --install . \
    && rm -rf /tmp/cpr

# 4. Directorio de trabajo
WORKDIR /app

# 5. Copiamos el código fuente
COPY . .

# 6. Compilación: el binario se llama CVC
RUN g++ -std=c++17 -o CVC CVC.cpp -I/usr/local/include -L/usr/local/lib -pthread -lcpr -ljson-c -lcurl -lssl -lcrypto

# 7. Ejecución automática del programa
CMD ["./CVC"]