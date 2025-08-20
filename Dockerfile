FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
      build-essential cmake git ca-certificates python3
WORKDIR /StorPool

# Improve cache hits by copying metadata first, then the rest
COPY CMakeLists.txt ./
COPY . .

# Configure and build
RUN bash -lc 'source project_configure && sp_project_build && sp_project_test'
