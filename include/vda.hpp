template<typename T>
struct VDA {
    T* data = nullptr;
    size_t count = 0;
    size_t capacity = 0;
};

template<typename T>
void vda_grow(VDA<T>& vda) {
    size_t new_capacity = (vda.capacity == 0) ? 256 : vda.capacity * 2;
    T* new_data = static_cast<T*>(std::realloc(vda.data, new_capacity * sizeof(T)));
    if (!new_data) {
        std::out << "VDA FAIL!" << std::endl;
    }
    vda.data = new_data;
    vda.capacity = new_capacity;
}

template<typename T>
void vda_append_one(VDA<T>& vda, const T& value) {
    if (vda.count >= vda.capacity)
        vda_grow(vda);
    vda.data[vda.count++] = value;
}

// Base case: 1 element
template<typename T>
void vda_append(VDA<T>& vda, const T& value) {
    vda_append_one(vda, value);
}

// Recursive case: multiple elements
template<typename T, typename... Args>
void vda_append(VDA<T>& vda, const T& value, const Args&... rest) {
    vda_append_one(vda, value);
    vda_append(vda, rest...);  // recurse
}

template<typename T>
void vda_free(VDA<T>& vda) {
    std::free(vda.data);
    vda.data = nullptr;
    vda.count = 0;
    vda.capacity = 0;
}
