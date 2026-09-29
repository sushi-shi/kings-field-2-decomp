//! Allocation-free codecs for King's Field II resource containers.
//!
//! The grammars here come from retail `GAME.EXE` resource-consumer
//! disassembly and the retail disc corpus. They intentionally do not call into
//! the matching C reconstruction: the Rust readers provide an independent
//! implementation for differential tests.
//!
//! All readers borrow their input and all writers use caller-owned buffers.
//! Multi-byte fields are decoded with explicit byte order, so the
//! result does not depend on host alignment or byte order.

#![no_std]
#![forbid(unsafe_code)]

pub mod cd_location;
pub mod chunked;
pub mod sector_archive;

#[derive(Debug)]
pub(crate) enum Sink<'a> {
    Count(usize),
    Write { bytes: &'a mut [u8], at: usize },
}

impl Sink<'_> {
    pub(crate) fn extend(&mut self, value: &[u8]) -> bool {
        match self {
            Self::Count(len) => match len.checked_add(value.len()) {
                Some(next) => {
                    *len = next;
                    true
                }
                None => false,
            },
            Self::Write { bytes, at } => {
                let Some(end) = at.checked_add(value.len()) else {
                    return false;
                };
                let Some(destination) = bytes.get_mut(*at..end) else {
                    return false;
                };
                destination.copy_from_slice(value);
                *at = end;
                true
            }
        }
    }

    pub(crate) fn zeros(&mut self, count: usize) -> bool {
        const ZEROS: [u8; 64] = [0; 64];
        let mut left = count;
        while left != 0 {
            let step = left.min(ZEROS.len());
            if !self.extend(&ZEROS[..step]) {
                return false;
            }
            left -= step;
        }
        true
    }

    pub(crate) fn len(&self) -> usize {
        match self {
            Self::Count(len) => *len,
            Self::Write { at, .. } => *at,
        }
    }
}
