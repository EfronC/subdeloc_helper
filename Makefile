build:
	python setup.py build_ext --inplace
wheel:
	python setup.py bdist_wheel
test:
	python -m unittest