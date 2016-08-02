#include <algorithm>
#include <iostream>
#include <list>
#include <map>

#include <glib.h>

using FooIdToBarMap = std::map<uint8_t, std::list<uint16_t>>;

class Foo 
{
public:
  Foo() = default;
  Foo(const FooIdToBarMap& map) : m_map(map) { }

  GVariant* Serialise() const;
  bool Deserialise(GVariant* input);

  bool operator==(const Foo& rhs) const 
  {
    return m_map == rhs.m_map;
  }

private:
  FooIdToBarMap m_map;
};

GVariant*
Foo::Serialise() const 
{  
  GVariantBuilder result;
  GVariantBuilder builder;
  g_variant_builder_init (&result, G_VARIANT_TYPE ("a(yaq)"));

  for (const auto& item : m_map) 
  {
    g_variant_builder_init (&builder, G_VARIANT_TYPE ("aq"));      
    for (auto bar : item.second) 
    {
      g_variant_builder_add (&builder, "q", bar);
    }

    g_variant_builder_add (&result, "(yaq)", item.first, &builder);
  }

  return g_variant_builder_end (&result);
}

bool Foo::Deserialise(GVariant* input) 
{
  for (gsize i = 0; i < g_variant_n_children (input); ++i) 
  {
    GVariant* item = g_variant_get_child_value (input, i);
    GVariantIter* iter;
    guchar fooId;

    g_variant_get (item, "(yaq)", &fooId, &iter);
    guint16 bar;
    while (g_variant_iter_loop (iter, "q", &bar)) 
    {
      m_map[fooId].push_back(bar);
    }

    g_variant_iter_free (iter);
    g_variant_unref (item);
  }

  return true;
}

int main(int argc, char** argv) 
{
  FooIdToBarMap map 
  {
    { 0, { 0x15, 0x16, 0x17 } },
    { 1, { 0x25, 0x26, 0x27 } },
  };

  Foo di(map);
  GVariant* sdi = di.Serialise();
  Foo other;
  if (other.Deserialise(sdi)) 
  {
    std::cout << (other == di ? "OK" : "Failed") << std::endl;
  }

  g_variant_unref (sdi);

  return 0;
}
