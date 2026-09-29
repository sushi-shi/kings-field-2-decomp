use kf_codec::cd_location::{bcd_to_int, int_to_bcd, CdLocation};

#[test]
fn bcd_round_trips_two_digit_values() {
    for value in 0..100u8 {
        assert_eq!(bcd_to_int(int_to_bcd(value)), u32::from(value));
    }
    assert_eq!(int_to_bcd(59), 0x59);
    assert_eq!(bcd_to_int(0x74), 74);
}

#[test]
fn out_of_range_values_keep_the_low_byte_of_the_retail_result() {
    // 200 / 10 << 4 | 200 % 10 = 0x140; the location byte store keeps 0x40.
    assert_eq!(int_to_bcd(200), 0x40);
}

#[test]
fn sectors_count_from_zero_without_a_lead_in() {
    let location = CdLocation::from_sector(2 * 4500 + 3 * 75 + 4);
    assert_eq!(location.to_bytes(), [0x02, 0x03, 0x04, 0x00]);
    assert_eq!(location.to_sector(), 2 * 4500 + 3 * 75 + 4);
    assert_eq!(CdLocation::from_sector(0).to_bytes(), [0; 4]);
}

#[test]
fn adding_carries_seconds_and_minutes_and_clears_the_track() {
    let base = CdLocation::from_bytes([0x00, 0x59, 0x74, 0x01]);
    assert_eq!(base.add(1).to_bytes(), [0x01, 0x00, 0x00, 0x00]);
    assert_eq!(base.add(76).to_bytes(), [0x01, 0x01, 0x00, 0x00]);
}
