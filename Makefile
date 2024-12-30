build:
	python setup.py build_ext --inplace
wheel:
	python setup.py bdist_wheel
test:
	python -m unittest
clean:
	python setup.py clean --all
	if [ -d "./dist" ]; then rm -R ./dist; fi
	if [ -d "./modify_subs.egg-info" ]; then rm -R ./modify_subs.egg-info; fi