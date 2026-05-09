# syntax = docker/dockerfile:1.2

FROM ros:jazzy-ros-base-noble AS core_system

# Prevent interactive questions during image build
ENV DEBIAN_FRONTEND=noninteractive

# Remove the docker-auto-clean feature of debian docker images
RUN rm -f /etc/apt/apt.conf.d/docker-clean

RUN userdel ubuntu

# Install essential packages and ROS development tools
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt update \
    && apt install -y --no-install-recommends \
    apt-transport-https \
    bash-completion \
    build-essential \
    ca-certificates \
    ccache \
    curl \
    gdb \
    git \
    gnupg \
    htop \
    iputils-ping \
    libglfw3-dev\
    locate \
    mc \
    nano \
    openssh-client \
    python-is-python3 \
    python3-colcon-argcomplete \
    python3-colcon-common-extensions \
    python3-pip \
    screen \
    sudo \
    tmux \
    vim \
    wget \
    zsh \
    libserial-dev \
    clang-tidy \
    clang-format \
    pre-commit \
    locales \
    libpcl-dev \
    libi2c-dev \
    python3-vcstool \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

FROM core_system AS cacher

WORKDIR /ros2_ws

COPY . /ros2_ws

RUN mkdir -p /tmp/ws/ros \
    && find ./ -name "package.xml" | xargs cp --parents -t /tmp/ws/ros \
    && find ./ -name "COLCON_IGNORE" | xargs cp --parents -t /tmp/ws/ros \
    || true

FROM core_system AS builder

RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt update \
    && apt install --no-install-recommends -y ros-$ROS_DISTRO-ros-base \
    && rm -rf /var/lib/apt/lists/*

ARG UID=1000
ARG GID=1000
ARG USER=dev
ARG GROUP=dev

RUN addgroup --gid $GID $GROUP \
    && adduser --uid $UID --ingroup $GROUP --home /home/$USER --shell /bin/bash --disabled-password --gecos "" $USER \
    && adduser $USER sudo \
    && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers \
    && echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> /home/$USER/.bashrc \
    && echo "source /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash" >> /home/$USER/.bashrc \
    && echo "export QT_X11_NO_MITSHM=1" >> /home/$USER/.bashrc

USER $USER

ENV HOME=/home/$USER
ENV WS_DIR=$HOME/ws

RUN mkdir -p $HOME/.cache
RUN mkdir -p $HOME/.config

WORKDIR $WS_DIR

# Install colcon mixin that will allow us to use ccache to speed up build
RUN colcon mixin add default https://raw.githubusercontent.com/colcon/colcon-mixin-repository/master/index.yaml \
    && colcon mixin update default
COPY --chown=$USER:$GROUP docker/colcon_defaults.yml /home/$USER/.colcon/defaults.yml
ENV CC="/usr/lib/ccache/gcc"
ENV CXX="/usr/lib/ccache/g++"

COPY --from=cacher --chown=$USER:$GROUP /tmp/ws/ros /home/$USER/ws/ros

RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    sudo apt update \
    && rosdep update \
    && rosdep install --from-paths ros --ignore-src --rosdistro $ROS_DISTRO -y

ENTRYPOINT [ "/bin/bash" ]