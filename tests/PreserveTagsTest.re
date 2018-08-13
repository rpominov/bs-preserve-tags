let test = (description: string, body: unit => bool) : unit => {
  Js.log("");
  Js.log(description);
  switch (body()) {
  | true => Js.log("Ok!")
  | false =>
    Js.log("Condition failed!");
    Js.log("");
    Node.Process.exit(1);
  | exception ex =>
    Js.log2("Threw", ex);
    Js.log("");
    Node.Process.exit(1);
  };
};

let transfer = [%bs.raw
  {| function(x) {return JSON.parse(JSON.stringify(x))} |}
];

let unsafeTransfer = (x: 'a) : 'a => x |> transfer;

let safeTransfer = x =>
  x |> PreserveTags.encode |> unsafeTransfer |> PreserveTags.decode;

type t('a) =
  | A('a)
  | B('a);

type record = {
  b: bool,
  i: int,
};

test("Variants transfer doesn't work without encoding", () =>
  unsafeTransfer(B(1)) == A(1)
);

test("Variants transfer does work with encoding", () =>
  safeTransfer(B(1)) == B(1)
);

test("Undefined transfer doesn't work without encoding", () =>
  unsafeTransfer([Js.Nullable.undefined]) == [Js.Nullable.null]
);

test("Undefined transfer does work with encoding", () =>
  safeTransfer([Js.Nullable.undefined]) == [Js.Nullable.undefined]
);

test("Complex data preserved", () => {
  let data = [
    `Foo(1),
    `Bar(A(B([|1, 2, 3|]))),
    `Baz({"foo": 1, "bar": false}),
    `Record({b: true, i: 100}),
  ];
  safeTransfer(data) == data;
});