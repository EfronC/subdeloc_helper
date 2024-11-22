# Modify Subtitles Helper
-----------------------------

Cython/C++ functions to be imported in other projects.

## Modify Subs

Module to load an ASS or SRT file, a words JSON, and apply the required replacements to the subtitles.

```
from c_delocalizer.modify_subs import overwrite_subs, modify_subs_py

generated_json = modify_subs_py(str(f), str(wordsfile)) # Opens file, execute replacements, and save result on a JSON file for further use.
generated_subfile = overwrite_subs(str(f), str(wordsfile)) # Same as before, but creates delocalized subtitle file instead of JSON.
```

## Pair Subs

Module to send a pair of intervals, and returns a list of pairs based on the time segment. You can define the tolerance margin through a `PAIR_MARGIN` variable in the .env.

```
from c_delocalizer.pair_subs import find_intersections

set_a = [
    {"start": 0, "end": 5000, "text":"Lorem Ipsum", "original":"Lorem Ipsum", "nl": 0},
    {"start": 5000, "end": 10000, "text":"Foo Bar", "original":"Foo Bar", "nl": 1},
]
set_b = [
    {"start": 0, "end": 5000, "text":"Lorem Ipsum", "original":"Lorem Ipsum", "nl": 0},
    {"start": 5000, "end": 10000, "text":"Foo Bar", "original":"Foo Bar", "nl": 1},
]

intersections = find_intersections(set_a, set_b)
```

Sister projects:
- [SubDelocalizer Tools](https://github.com/EfronC/subdeloc_tools)
- [Delocalizer](https://github.com/EfronC/Delocalizer)
