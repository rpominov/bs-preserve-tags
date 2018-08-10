# PreserveTags

A hacky workaround to allow BuckleScript data structures to be transferred via JSON.

BuckleScript represents variants as JavaScript arrays with an added property `tag`, like so:

```js
const arr = [1]; 
arr.tag = 0;
```

This representation cannot be transferred via JSON, if we do `JSON.stringify(arr)`, the `tag` will be stripped. PreserveTags makes it possible:

```reason

let transfer = [%bs.raw
  {| function(x) {return JSON.parse(JSON.stringify(x))} |}
];

type t('a) =
  | A('a)
  | B('a);

/* Without PreserveTags B may become A */
transfer(B(1)) == A(1) /* true */

/* With PreserveTags it works fine */
PreserveTags.decode(transfer(PreserveTags.encode(B(1)))) == B(1) /* true */
```

## API

```reason
module PreserveTags = {
  type t('a);
  let encode: 'a => t('a);
  let decode: t('a) => 'a;
}
```

## Installation

TODO (it isn't actually published yet)

```
npm i @rpominov/bs-preserve-tags
```

Then in your `bsconfig.json`:

```
"bs-dependencies": ["@rpominov/bs-preserve-tags"]
```