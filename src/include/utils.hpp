#pragma once

namespace util {

    template <class T>
    constexpr const T &MINConst(const T &a, const T &b) {
        return a >= b ? b : a;
    }
    template <class T>
    constexpr const T &MAXConst(const T &a, const T &b) {
        return a >= b ? a : b;
    }
    template <class T>
    inline __attribute__((always_inline)) const T &MIN(const T &a, const T &b) {
        return a >= b ? b : a;
    }
    template <class T>
    inline __attribute__((always_inline)) const T &MAX(const T &a, const T &b) {
        return a >= b ? a : b;
    }

    template <class SignT, class T, uint8_t usedBits>
    SignT signExt(T val) {
        // Let the compiler handle the sign extension as it should now whats best for doing so!
        struct {
            SignT x : usedBits;
        } s;
        return s.x = val;
    }

    template <class T, uint8_t OutInt, uint8_t OutFrac, uint8_t InInt, uint8_t InFrac, class InType>
    T fixedToFixed(InType f) {
        static_assert(1 + OutInt + OutFrac <= sizeof(T) * 8, "Sign, integer and fraction take up more bits than out type T provides.");
        static_assert(1 + InInt + InFrac <= sizeof(InType) * 8, "Sign, integer and fraction take up more bits than in type InType provides.");

        constexpr int8_t diff = OutFrac - InFrac;
        T sgExtended;
        if (diff < 0) {
            sgExtended = (signExt<InType, InType, InFrac + InInt + 1>(f)) >> (-diff);
        } else {
            sgExtended = signExt<T, InType, InFrac + InInt + 1>(f) << diff;
        }

        constexpr T mask = (static_cast<T>(1) << (OutFrac + OutInt + 1)) - 1;
        return sgExtended & mask;
    }

    template <class T, class ImmT, uint8_t Int, uint8_t Frac>
    T intraFixedMul(T a, T b) {
        static_assert(1 + Int + Frac <= sizeof(T) * 8, "Sign, integer and fraction take up more bits than T provides.");

        ImmT sgExtA = signExt<ImmT, Int + Frac + 1>(a);
        ImmT sgExtB = signExt<ImmT, Int + Frac + 1>(b);

        ImmT res = sgExtA * sgExtB;
        res >>= Frac;

        constexpr T mask = (static_cast<T>(1) << (Frac + Int + 1)) - 1;
        return res & mask;
    }

} // namespace util