/// SPDX-License-Identifier: CC0-1.0
///////////////////////////////////////////////////////////////////////////////
/// @file daisy_helpers.h
/// @brief
/// @author pfburdette <paul.f.burdette@gmail.com>
///
/// @copyright This work is dedicated to the public domain under CC0 1.0.
/// To the extent possible under law, the author(s) have waived all copyright
/// and related or neighboring rights to this software.
/// See <http://creativecommons.org>
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <daisy.h>

///////////////////////////////////////////////////////////////////////////////
// Add equality/inequality operators to mapped values
///////////////////////////////////////////////////////////////////////////////

bool
operator==(const daisy::MappedFloatValue& lhs,
           const daisy::MappedFloatValue& rhs)
{
  return lhs.Get() == rhs.Get();
}

bool
operator!=(const daisy::MappedFloatValue& lhs,
           const daisy::MappedFloatValue& rhs)
{
  return !(lhs == rhs);
}

bool
operator==(const daisy::MappedIntValue& lhs, const daisy::MappedIntValue& rhs)
{
  return lhs.Get() == rhs.Get();
}

bool
operator!=(const daisy::MappedIntValue& lhs, const daisy::MappedIntValue& rhs)
{
  return !(lhs == rhs);
}

bool
operator==(const daisy::MappedStringListValue& lhs,
           const daisy::MappedStringListValue& rhs)
{
  return lhs.GetIndex() == rhs.GetIndex();
}

bool
operator!=(const daisy::MappedStringListValue& lhs,
           const daisy::MappedStringListValue& rhs)
{
  return !(lhs == rhs);
}