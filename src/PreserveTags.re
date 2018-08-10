type t('a);

%bs.raw
{|
  var tagRep = "tag7551544577058851"
  var undefinedRep = "undefined6940448865807247"
|};

let encode = [%bs.raw
  {|
  function encode(data) {
    if (Array.isArray(data)) {
      var encoded = data.map(encode)
      if (data.tag !== undefined) {
        var result = {array: encoded}
        result[tagRep] = data.tag
        return result
      } else {
        return encoded
      }
    }

    if (typeof data === "object" && data !== null) {
      var result = {}
      for (var key in data) {
        result[key] = encode(data[key])
      }
      return result
    }

    if (data === undefined) {
      return undefinedRep
    }

    return data
  }
|}
];

let decode = [%bs.raw
  {|
  function decode(data) {
    if (typeof data === "object" && data !== null) {
      if (data[tagRep] !== undefined) {
        var result = data.array.map(decode)
        result.tag = data[tagRep]
        return result
      } else {
        if (Array.isArray(data)) {
          return data.map(decode)
        } else {
          var result = {}
          for (var key in data) {
            result[key] = decode(data[key])
          }
          return result
        }
      }
    }

    if (data === undefinedRep) {
      return undefined
    }

    return data
  }
|}
];