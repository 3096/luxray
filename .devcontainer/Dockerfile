FROM devkitpro/devkita64:latest

RUN apt-get update && apt-get install -y \
    git \
    clang-format \
    && rm -rf /var/lib/apt/lists/*
RUN dkp-pacman -Syyu --noconfirm switch-dev switch-tools
