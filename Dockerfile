FROM apiaryio/emcc:1.38.11

# ------------------------------------------------------------------------------

RUN echo "\n## Start building" \
  \
  &&	echo "\n## Update and install packages" \
  &&	apt-get -qq -y update && apt-get -qq install -y --no-install-recommends \
  ruby \
  git \
  clang \
  bison \
  build-essential  \
  &&	echo "\n## Cleaning up" \
  &&	apt-get -y clean \
  &&	apt-get -y autoclean \
  &&	apt-get -y autoremove \
  \
  &&	rm -rf /var/lib/apt/lists/* \
  &&	rm -rf /var/cache/debconf/*-old \
  &&	rm -rf /usr/share/doc/* \
  &&	rm -rf /usr/share/man/?? \
  &&	rm -rf /usr/share/man/??_* \
  &&	cp -R /usr/share/locale/en\@* /tmp/ && rm -rf /usr/share/locale/* && mv /tmp/en\@* /usr/share/locale/ \
  \
  &&	echo "\n## Done"