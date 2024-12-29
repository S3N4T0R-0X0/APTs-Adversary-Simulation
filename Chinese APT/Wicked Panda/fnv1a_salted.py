def fnv1a_salted(data, salt, seed_value=0x811C9DC5):
    _data = data + salt
    _hash = seed_value
    prime = 0x01000193
    for byte in _data:
        _hash ^= byte
        _hash *= prime
        _hash &= 0xFFFFFFFF  # Ensure it stays within 32 bits
    return _hash

# Test data
ntdll = b"n\x00t\x00d\x00l\x00l\x00"
ldrloaddll = b"LdrLoadDll"
salt = b"\xba\xb4\x24\xcb"

# Calculate hashes
ntdll_hash = fnv1a_salted(ntdll, salt)
ldrloaddll_hash = fnv1a_salted(ldrloaddll, salt)

# Print results
print(f"Hash for 'ntdll': {hex(ntdll_hash)}")  # Expected: 0xfe0b07b0
print(f"Hash for 'LdrLoadDll': {hex(ldrloaddll_hash)}")  # Expected: 0xca7bb6ac

