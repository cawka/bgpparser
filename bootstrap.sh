git log --format="%an <%ae>" |sort|uniq > AUTHORS
python bootstrap/gitlog2changelog.py 

autoreconf -I m4/ --install

